#include "engine/server_engine.h"

#include "common/global_settings.hpp"
#include "protocol/protocol.hpp"

#include <atomic>
#include <chrono>
#include <csignal>
#include <iostream>
#include <thread>
#include <variant>

namespace {

constexpr int kTickHz = 20;
constexpr float kDt = 1.0f / static_cast<float>(kTickHz);
constexpr auto kTickPeriod = std::chrono::milliseconds(1000 / kTickHz);

std::atomic<botf::ServerEngine*> gActiveEngine{nullptr};

void onSignal(int) {
	if (botf::ServerEngine* engine = gActiveEngine.load()) {
		engine->requestStop();
	}
}

botf::common::WorldSnapshot buildWorldSnapshot(const botf::GameLogic& game, std::uint32_t tick) {
	botf::common::WorldSnapshot snapshot;
	snapshot.tick = tick;
	snapshot.players.reserve(game.objects().size());
	for (const auto& obj : game.objects()) {
		snapshot.players.push_back(botf::common::PlayerPose{obj.id(), obj.position(), obj.angle()});
	}
	return snapshot;
}

}  // namespace

botf::ServerEngine::ServerEngine() : m_server(m_inbound, m_outbound) {}

botf::ServerEngine::~ServerEngine() {
	requestStop();
	shutdown();
	if (gActiveEngine.load() == this) {
		gActiveEngine.store(nullptr);
	}
}

void botf::ServerEngine::requestStop() {
	m_running = false;
}

bool botf::ServerEngine::startListen(std::uint16_t port) {
	return m_server.listen(port);
}

void botf::ServerEngine::shutdown() {
	m_server.stop();
}

void botf::ServerEngine::pollNetwork() {
	m_server.poll();
}

std::uint16_t botf::ServerEngine::port() const {
	return m_server.port();
}

std::size_t botf::ServerEngine::clientCount() const {
	return m_server.clientCount();
}

const botf::GameLogic& botf::ServerEngine::game() const {
	return m_game;
}

std::uint32_t botf::ServerEngine::simTick() const {
	return m_simTick;
}

std::optional<botf::common::MoveCommand> botf::ServerEngine::lastMove(ClientId clientId) const {
	const auto it = m_inputStates.find(clientId);
	if (it == m_inputStates.end()) {
		return std::nullopt;
	}
	return it->second.command;
}

void botf::ServerEngine::ensureClientBound(ClientId clientId) {
	if (m_game.entityForClient(clientId).has_value()) {
		return;
	}

	if (!m_game.objects().empty() && clientId == m_game.objects().front().id()) {
		m_game.bindClient(clientId, m_game.objects().front().id());
		return;
	}

	const common::ID entity = m_game.spawnPlayer(
		common::Point{common::GlobalSettings::defaultSpawnX, common::GlobalSettings::defaultSpawnY},
		common::GlobalSettings::defaultPlayerSpeedLimit);
	m_game.bindClient(clientId, entity);
}

void botf::ServerEngine::applyInboundMoves(bool verbose) {
	while (auto note = m_inbound.tryPop()) {
		auto decoded = protocol::decode(note->payload);
		if (!decoded) {
			std::cerr << "[server] bad envelope from client " << note->clientId << '\n';
			continue;
		}

		const auto* move = std::get_if<protocol::DecodedMove>(&*decoded);
		if (!move) {
			if (verbose) {
				std::cout << "[server] ignore non-Move from client " << note->clientId << '\n';
			}
			continue;
		}

		m_inputStates[note->clientId] = ClientInputState{move->command, move->sequence};

		ensureClientBound(note->clientId);
		m_game.setClientInput(note->clientId, move->command.dx, move->command.dy);

		if (verbose) {
			std::cout << "[server] Move state client=" << note->clientId << " "
					  << protocol::toString(move->command) << " dx=" << move->command.dx
					  << " dy=" << move->command.dy << " tick=" << move->command.tick
					  << " seq=" << move->sequence << '\n';
		}
	}
}

void botf::ServerEngine::enqueueWorldState(std::uint32_t tick, std::uint64_t sequence) {
	m_outbound.push(net::OutboundNotification{
		std::nullopt,
		protocol::encodeWorldSnapshot(buildWorldSnapshot(m_game, tick), sequence),
	});
}

void botf::ServerEngine::advanceSim(float dt) {
	m_game.tick(dt);
	++m_simTick;
	enqueueWorldState(m_simTick, m_simTick);
}

int botf::ServerEngine::runLoop() {
	std::cout << "[server] running on port " << port() << " at " << kTickHz
			  << " Hz (Ctrl+C to stop)\n";
	std::cout << "[server] Move → GameLogic.setClientInput → tick(" << kDt
			  << ") → WorldState broadcast\n";

	m_running = true;
	while (m_running) {
		const auto tickStart = std::chrono::steady_clock::now();

		pollNetwork();
		applyInboundMoves(true);
		advanceSim(kDt);
		pollNetwork();

		if (m_simTick % static_cast<std::uint32_t>(kTickHz) == 0) {
			const auto& objects = m_game.objects();
			std::cout << "[server] heartbeat tick=" << m_simTick << " clients=" << clientCount()
					  << " entities=" << objects.size();
			if (!objects.empty()) {
				const auto p = objects.front().position();
				std::cout << " p0=(" << p.x << "," << p.y << ")";
			}
			std::cout << '\n';
		}

		const auto elapsed = std::chrono::steady_clock::now() - tickStart;
		if (elapsed < kTickPeriod) {
			std::this_thread::sleep_for(kTickPeriod - elapsed);
		}
	}

	std::cout << "[server] shutting down\n";
	return 0;
}

int botf::ServerEngine::run(std::uint16_t port) {
	gActiveEngine.store(this);
	std::signal(SIGINT, onSignal);
	std::signal(SIGTERM, onSignal);

	if (!startListen(port)) {
		gActiveEngine.store(nullptr);
		return 1;
	}

	const int code = runLoop();
	shutdown();
	gActiveEngine.store(nullptr);
	return code;
}
