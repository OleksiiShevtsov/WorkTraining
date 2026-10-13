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

std::atomic<ServerEngine*> g_active_engine{nullptr};

void on_signal(int) {
  if (ServerEngine* engine = g_active_engine.load()) {
    engine->request_stop();
  }
}

bool send_framed(sf::TcpSocket& socket, const std::string& envelope_bytes) {
  const auto packet = net::frame_payload(envelope_bytes);
  return socket.send(packet.data(), packet.size()) == sf::Socket::Status::Done;
}

WorldState build_world_state(const GameLogic& game, std::uint32_t tick) {
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

bool recv_one_frame(sf::TcpSocket& socket,
                    std::vector<std::uint8_t>& buffer,
                    std::string& out_payload,
                    int attempts) {
  for (int i = 0; i < attempts; ++i) {
    if (auto payload = net::try_consume_frame(buffer)) {
      out_payload = std::move(*payload);
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
  request_stop();
  shutdown();
  if (g_active_engine.load() == this) {
    g_active_engine.store(nullptr);
  }
}

void ServerEngine::request_stop() {
  m_running = false;
}

bool ServerEngine::start_listen(std::uint16_t port) {
  return m_server.listen(port);
}

void ServerEngine::shutdown() {
  m_server.stop();
}

void ServerEngine::ensure_client_bound(ClientId client_id) {
  if (m_game.entity_for_client(client_id).has_value()) {
    return;
  }

  if (!m_game.objects().empty() && client_id == m_game.objects().front().id()) {
    m_game.bind_client(client_id, m_game.objects().front().id());
    return;
  }

  const common::ID entity = m_game.spawn_player(
      common::Point{common::GlobalSettings::defaultSpawnX, common::GlobalSettings::defaultSpawnY},
      common::GlobalSettings::defaultPlayerSpeedLimit);
  m_game.bind_client(client_id, entity);
}

void ServerEngine::apply_inbound_move(bool verbose) {
  while (auto note = m_inbound.try_pop()) {
    Envelope envelope;
    if (!envelope.ParseFromString(note->payload)) {
      std::cerr << "[server] bad envelope from client " << note->client_id << '\n';
      continue;
    }

    if (envelope.type() != MESSAGE_TYPE_MOVE) {
      if (verbose) {
        std::cout << "[server] ignore type=" << envelope.type() << " from client "
                  << note->client_id << '\n';
      }
      continue;
    }

    Move move;
    if (!move.ParseFromString(envelope.payload())) {
      std::cerr << "[server] bad Move payload from client " << note->client_id << '\n';
      continue;
    }

    move = net::normalize_move(move);
    m_input_states[note->client_id] = ClientInputState{move, envelope.sequence()};

    ensure_client_bound(note->client_id);
    m_game.set_client_input(note->client_id, move.dx(), move.dy());

    if (verbose) {
      std::cout << "[server] Move state client=" << note->client_id << " " << net::to_string(move)
                << " dx=" << move.dx() << " dy=" << move.dy() << " tick=" << move.tick()
                << " seq=" << envelope.sequence() << '\n';
    }
  }
}

void ServerEngine::enqueue_world_state(std::uint32_t tick, std::uint64_t sequence) {
  m_outbound.push(net::OutboundNotification{
      std::nullopt,
      net::make_world_state_envelope(build_world_state(m_game, tick), sequence),
  });
}

int ServerEngine::run_loop() {
  std::cout << "[server] running on port " << m_server.port() << " at " << kTickHz
            << " Hz (Ctrl+C to stop)\n";
  std::cout << "[server] Move → GameLogic.set_client_input → tick(" << kDt
            << ") → WorldState broadcast\n";

  std::uint64_t server_tick = 0;
  m_running = true;
  while (m_running) {
    const auto tick_start = std::chrono::steady_clock::now();

    m_server.poll();
    apply_inbound_move(true);
    m_game.tick(kDt);
    ++server_tick;
    enqueue_world_state(static_cast<std::uint32_t>(server_tick), server_tick);
    m_server.poll();

    if (server_tick % static_cast<std::uint64_t>(kTickHz) == 0) {
      const auto& objects = m_game.objects();
      std::cout << "[server] heartbeat tick=" << server_tick
                << " clients=" << m_server.client_count() << " entities=" << objects.size();
      if (!objects.empty()) {
        const auto p = objects.front().position();
        std::cout << " p0=(" << p.x << "," << p.y << ")";
      }
      std::cout << '\n';
    }

    const auto elapsed = std::chrono::steady_clock::now() - tick_start;
    if (elapsed < kTickPeriod) {
      std::this_thread::sleep_for(kTickPeriod - elapsed);
    }
  }

  std::cout << "[server] shutting down\n";
  return 0;
}

int ServerEngine::run(std::uint16_t port) {
  g_active_engine.store(this);
  std::signal(SIGINT, on_signal);
  std::signal(SIGTERM, on_signal);

  if (!start_listen(port)) {
    g_active_engine.store(nullptr);
    return 1;
  }

  const int code = run_loop();
  shutdown();
  g_active_engine.store(nullptr);
  return code;
}

int ServerEngine::run_smoke(std::uint16_t port) {
  if (!start_listen(port)) {
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
  std::uint32_t sim_tick = 0;
  std::vector<std::uint8_t> recv_buffer;

  for (const auto& sample : samples) {
    if (!send_framed(client,
                     net::make_move_envelope(sample.dx, sample.dy, sample.tick, sample.seq))) {
      std::cerr << "[server] smoke client failed to send Move\n";
      shutdown();
      return 1;
    }

    bool got = false;
    for (int i = 0; i < 100 && !got; ++i) {
      m_server.poll();
      apply_inbound_move(true);
      if (m_input_states.count(1) && m_input_states[1].move.tick() == sample.tick) {
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
      ++sim_tick;
      enqueue_world_state(sim_tick, sim_tick);
      m_server.poll();
    }
  }

  enqueue_world_state(sim_tick, sim_tick);
  for (int i = 0; i < 20; ++i) {
    m_server.poll();
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
  }

  client.setBlocking(false);

  WorldState last_state;
  bool got_world = false;
  for (int i = 0; i < 500; ++i) {
    m_server.poll();
    std::string payload;
    if (!recv_one_frame(client, recv_buffer, payload, 1)) {
      if (got_world && i > 50) {
        break;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(2));
      continue;
    }
    auto envelope = net::parse_envelope(payload);
    if (!envelope || envelope->type() != MESSAGE_TYPE_WORLD_STATE) {
      continue;
    }
    auto state = net::parse_world_state_payload(envelope->payload());
    if (!state || state->players_size() == 0) {
      continue;
    }
    last_state = std::move(*state);
    got_world = true;
  }

  client.disconnect();

  if (!m_input_states.count(1) || m_input_states[1].move.tick() != 4 ||
      !net::is_idle(m_input_states[1].move)) {
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

  if (!got_world) {
    std::cerr << "[server] smoke failed: no WorldState received by client\n";
    shutdown();
    return 1;
  }

  const auto& p0 = last_state.players(0);
  const float dx = p0.position().x() - end.x;
  const float dy = p0.position().y() - end.y;
  if (dx * dx + dy * dy > 0.01f) {
    std::cerr << "[server] smoke failed: WorldState pos mismatch " << "wire=(" << p0.position().x()
              << "," << p0.position().y() << ") " << "logic=(" << end.x << "," << end.y << ")\n";
    shutdown();
    return 1;
  }

  std::cout << "[server] WorldState ok tick=" << last_state.tick()
            << " players=" << last_state.players_size() << " p0=(" << p0.position().x() << ","
            << p0.position().y() << ")\n";
  std::cout << "BattleOfTheFlatland server — smoke ok (Move + GameLogic + WorldState)\n";
  shutdown();
  return 0;
}

}  // namespace botf
