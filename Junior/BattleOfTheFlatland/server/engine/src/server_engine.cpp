#include "engine/server_engine.h"

#include "common/global_settings.hpp"
#include "envelope.pb.h"
#include "net/packet_framing.hpp"
#include "net/protocol.hpp"
#include "world_state.pb.h"

#include <array>
#include <atomic>
#include <chrono>
#include <csignal>
#include <iostream>
#include <thread>
#include <vector>

#include <SFML/Network.hpp>

namespace botf {
namespace {

constexpr int kTickHz = 20;
constexpr float kDt = 1.0f / static_cast<float>(kTickHz);
constexpr auto kTickPeriod = std::chrono::milliseconds(1000 / kTickHz);

std::atomic<ServerEngine*> gActiveEngine{nullptr};

void onSignal(int) {
  if (ServerEngine* engine = gActiveEngine.load()) {
    engine->requestStop();
  }
}

bool sendFramed(sf::TcpSocket& socket, const std::string& envelopeBytes) {
  const auto packet = net::framePayload(envelopeBytes);
  return socket.send(packet.data(), packet.size()) == sf::Socket::Status::Done;
}

WorldState buildWorldState(const GameLogic& game, std::uint32_t tick) {
  WorldState state;
  state.set_tick(tick);
  for (const auto& obj : game.objects()) {
    PlayerState* player = state.add_players();
    player->set_id(obj.id());
    auto* position = player->mutable_position();
    position->set_x(obj.position().x);
    position->set_y(obj.position().y);
    player->set_aim_angle(obj.angle());
  }
  return state;
}

bool recvOneFrame(sf::TcpSocket& socket,
                  std::vector<std::uint8_t>& buffer,
                  std::string& outPayload,
                  int attempts) {
  for (int i = 0; i < attempts; ++i) {
    if (auto payload = net::tryConsumeFrame(buffer)) {
      outPayload = std::move(*payload);
      return true;
    }

    std::array<char, 4096> chunk{};
    std::size_t received = 0;
    const auto status = socket.receive(chunk.data(), chunk.size(), received);
    if (status == sf::Socket::Status::Done && received > 0) {
      buffer.insert(buffer.end(), chunk.data(), chunk.data() + received);
      continue;
    }
    if (status == sf::Socket::Status::Disconnected || status == sf::Socket::Status::Error) {
      return false;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
  }
  return false;
}

}  // namespace

ServerEngine::ServerEngine() : m_server(m_inbound, m_outbound) {}

ServerEngine::~ServerEngine() {
  requestStop();
  shutdown();
  if (gActiveEngine.load() == this) {
    gActiveEngine.store(nullptr);
  }
}

void ServerEngine::requestStop() {
  m_running = false;
}

bool ServerEngine::startListen(std::uint16_t port) {
  return m_server.listen(port);
}

void ServerEngine::shutdown() {
  m_server.stop();
}

void ServerEngine::ensureClientBound(ClientId clientId) {
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

void ServerEngine::applyInboundMove(bool verbose) {
  while (auto note = m_inbound.tryPop()) {
    Envelope envelope;
    if (!envelope.ParseFromString(note->payload)) {
      std::cerr << "[server] bad envelope from client " << note->clientId << '\n';
      continue;
    }

    if (envelope.type() != MESSAGE_TYPE_MOVE) {
      if (verbose) {
        std::cout << "[server] ignore type=" << envelope.type() << " from client " << note->clientId
                  << '\n';
      }
      continue;
    }

    Move move;
    if (!move.ParseFromString(envelope.payload())) {
      std::cerr << "[server] bad Move payload from client " << note->clientId << '\n';
      continue;
    }

    move = net::normalizeMove(move);
    m_inputStates[note->clientId] = ClientInputState{move, envelope.sequence()};

    ensureClientBound(note->clientId);
    m_game.setClientInput(note->clientId, move.dx(), move.dy());

    if (verbose) {
      std::cout << "[server] Move state client=" << note->clientId << " " << net::toString(move)
                << " dx=" << move.dx() << " dy=" << move.dy() << " tick=" << move.tick()
                << " seq=" << envelope.sequence() << '\n';
    }
  }
}

void ServerEngine::enqueueWorldState(std::uint32_t tick, std::uint64_t sequence) {
  m_outbound.push(net::OutboundNotification{
      std::nullopt,
      net::makeWorldStateEnvelope(buildWorldState(m_game, tick), sequence),
  });
}

int ServerEngine::runLoop() {
  std::cout << "[server] running on port " << m_server.port() << " at " << kTickHz
            << " Hz (Ctrl+C to stop)\n";
  std::cout << "[server] Move → GameLogic.setClientInput → tick(" << kDt
            << ") → WorldState broadcast\n";

  std::uint64_t serverTick = 0;
  m_running = true;
  while (m_running) {
    const auto tickStart = std::chrono::steady_clock::now();

    m_server.poll();
    applyInboundMove(true);
    m_game.tick(kDt);
    ++serverTick;
    enqueueWorldState(static_cast<std::uint32_t>(serverTick), serverTick);
    m_server.poll();

    if (serverTick % static_cast<std::uint64_t>(kTickHz) == 0) {
      const auto& objects = m_game.objects();
      std::cout << "[server] heartbeat tick=" << serverTick << " clients=" << m_server.clientCount()
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

int ServerEngine::run(std::uint16_t port) {
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

int ServerEngine::runSmoke(std::uint16_t port) {
  if (!startListen(port)) {
    return 1;
  }

  sf::TcpSocket client;
  client.setBlocking(true);
  if (client.connect(sf::IpAddress::LocalHost, port, sf::seconds(2)) != sf::Socket::Status::Done) {
    std::cerr << "[server] smoke client failed to connect\n";
    shutdown();
    return 1;
  }

  struct Sample {
    int dx;
    int dy;
    std::uint32_t tick;
    std::uint64_t seq;
  };
  const Sample samples[] = {
      {0, 1, 1, 1},
      {1, 1, 2, 2},
      {-1, 0, 3, 3},
      {0, 0, 4, 4},
  };

  const auto start = m_game.objects().front().position();
  std::uint32_t simTick = 0;
  std::vector<std::uint8_t> recvBuffer;

  for (const auto& sample : samples) {
    if (!sendFramed(client, net::makeMoveEnvelope(sample.dx, sample.dy, sample.tick, sample.seq))) {
      std::cerr << "[server] smoke client failed to send Move\n";
      shutdown();
      return 1;
    }

    bool got = false;
    for (int i = 0; i < 100 && !got; ++i) {
      m_server.poll();
      applyInboundMove(true);
      if (m_inputStates.count(1) && m_inputStates[1].move.tick() == sample.tick) {
        got = true;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
    if (!got) {
      std::cerr << "[server] smoke failed waiting for Move tick=" << sample.tick << '\n';
      shutdown();
      return 1;
    }

    for (int j = 0; j < 15; ++j) {
      m_game.tick(kDt);
      ++simTick;
      enqueueWorldState(simTick, simTick);
      m_server.poll();
    }
  }

  enqueueWorldState(simTick, simTick);
  for (int i = 0; i < 20; ++i) {
    m_server.poll();
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
  }

  client.setBlocking(false);

  WorldState lastState;
  bool gotWorld = false;
  for (int i = 0; i < 500; ++i) {
    m_server.poll();
    std::string payload;
    if (!recvOneFrame(client, recvBuffer, payload, 1)) {
      if (gotWorld && i > 50) {
        break;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(2));
      continue;
    }
    auto envelope = net::parseEnvelope(payload);
    if (!envelope || envelope->type() != MESSAGE_TYPE_WORLD_STATE) {
      continue;
    }
    auto state = net::parseWorldStatePayload(envelope->payload());
    if (!state || state->players_size() == 0) {
      continue;
    }
    lastState = std::move(*state);
    gotWorld = true;
  }

  client.disconnect();

  if (!m_inputStates.count(1) || m_inputStates[1].move.tick() != 4 ||
      !net::isIdle(m_inputStates[1].move)) {
    std::cerr << "[server] smoke failed: expected last state IDLE tick=4\n";
    shutdown();
    return 1;
  }

  const auto end = m_game.objects().front().position();
  std::cout << "[server] logic pos start=(" << start.x << "," << start.y << ") end=(" << end.x
            << "," << end.y << ")\n";

  if (end.x == start.x && end.y == start.y) {
    std::cerr << "[server] smoke failed: DynamicObject did not move\n";
    shutdown();
    return 1;
  }

  if (!gotWorld) {
    std::cerr << "[server] smoke failed: no WorldState received by client\n";
    shutdown();
    return 1;
  }

  const auto& p0 = lastState.players(0);
  const float dx = p0.position().x() - end.x;
  const float dy = p0.position().y() - end.y;
  if (dx * dx + dy * dy > 0.01f) {
    std::cerr << "[server] smoke failed: WorldState pos mismatch " << "wire=(" << p0.position().x()
              << "," << p0.position().y() << ") " << "logic=(" << end.x << "," << end.y << ")\n";
    shutdown();
    return 1;
  }

  std::cout << "[server] WorldState ok tick=" << lastState.tick()
            << " players=" << lastState.players_size() << " p0=(" << p0.position().x() << ","
            << p0.position().y() << ")\n";
  std::cout << "BattleOfTheFlatland server — smoke ok (Move + GameLogic + WorldState)\n";
  shutdown();
  return 0;
}

}  // namespace botf
