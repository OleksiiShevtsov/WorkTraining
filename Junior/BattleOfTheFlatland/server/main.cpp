#include "common/global_settings.hpp"
#include "common/types.hpp"
#include "envelope.pb.h"
#include "logic/game_logic.h"
#include "move.pb.h"
#include "net/packet_framing.hpp"
#include "net/protocol.hpp"
#include "net/tcp_server.hpp"
#include "world_state.pb.h"

#include <array>
#include <atomic>
#include <chrono>
#include <csignal>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <thread>
#include <unordered_map>
#include <vector>

#include <SFML/Network.hpp>

namespace botf {
namespace {

constexpr std::uint16_t kPort = 54545;
constexpr int kTickHz = 20;
constexpr float kDt = 1.0f / static_cast<float>(kTickHz);
constexpr auto kTickPeriod = std::chrono::milliseconds(1000 / kTickHz);

std::atomic<bool> g_running{true};

void on_signal(int) {
  g_running = false;
}

bool send_framed(sf::TcpSocket& socket, const std::string& envelope_bytes) {
  const auto packet = net::frame_payload(envelope_bytes);
  return socket.send(packet.data(), packet.size()) == sf::Socket::Status::Done;
}

struct ClientInputState {
  Move move{};
  std::uint64_t sequence = 0;
};

void ensure_client_bound(GameLogic& game, ClientId client_id) {
  if (game.entity_for_client(client_id).has_value()) {
    return;
  }

  // First connected client reuses the default spawned player when ids match.
  if (!game.objects().empty() && client_id == game.objects().front().id()) {
    game.bind_client(client_id, game.objects().front().id());
    return;
  }

  const common::ID entity = game.spawn_player(
      common::Point{common::GlobalSettings::defaultSpawnX, common::GlobalSettings::defaultSpawnY},
      common::GlobalSettings::defaultPlayerSpeedLimit);
  game.bind_client(client_id, entity);
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

void enqueue_world_state(net::OutboundQueue& outbound,
                         const GameLogic& game,
                         std::uint32_t tick,
                         std::uint64_t sequence) {
  // Broadcast: client_id == nullopt → every connected client.
  outbound.push(net::OutboundNotification{
      std::nullopt,
      net::make_world_state_envelope(build_world_state(game, tick), sequence),
  });
}

void apply_inbound_move(net::InboundQueue& inbound,
                        std::unordered_map<ClientId, ClientInputState>& states,
                        GameLogic& game,
                        bool verbose) {
  while (auto note = inbound.try_pop()) {
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
    states[note->client_id] = ClientInputState{move, envelope.sequence()};

    ensure_client_bound(game, note->client_id);
    game.set_client_input(note->client_id, move.dx(), move.dy());

    if (verbose) {
      std::cout << "[server] Move state client=" << note->client_id << " " << net::to_string(move)
                << " dx=" << move.dx() << " dy=" << move.dy() << " tick=" << move.tick()
                << " seq=" << envelope.sequence() << '\n';
    }
  }
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

int run_smoke(net::TcpServer& server,
              net::InboundQueue& inbound,
              net::OutboundQueue& outbound,
              std::unordered_map<ClientId, ClientInputState>& states,
              GameLogic& game) {
  sf::TcpSocket client;
  client.setBlocking(true);
  if (client.connect(sf::IpAddress::LocalHost, kPort, sf::seconds(2)) != sf::Socket::Status::Done) {
    std::cerr << "[server] smoke client failed to connect\n";
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

  const auto start = game.objects().front().position();
  std::uint32_t sim_tick = 0;
  std::vector<std::uint8_t> recv_buffer;

  // Hold each Move state for several sim ticks (matches real client cadence).
  for (const auto& sample : samples) {
    if (!send_framed(client,
                     net::make_move_envelope(sample.dx, sample.dy, sample.tick, sample.seq))) {
      std::cerr << "[server] smoke client failed to send Move\n";
      return 1;
    }

    bool got = false;
    for (int i = 0; i < 100 && !got; ++i) {
      server.poll();
      apply_inbound_move(inbound, states, game, true);
      if (states.count(1) && states[1].move.tick() == sample.tick) {
        got = true;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
    if (!got) {
      std::cerr << "[server] smoke failed waiting for Move tick=" << sample.tick << '\n';
      return 1;
    }

    for (int j = 0; j < 15; ++j) {
      game.tick(kDt);
      ++sim_tick;
      enqueue_world_state(outbound, game, sim_tick, sim_tick);
      server.poll();
    }
  }

  // Final snapshot, then drain every WorldState and keep the last (authoritative end pos).
  enqueue_world_state(outbound, game, sim_tick, sim_tick);
  for (int i = 0; i < 20; ++i) {
    server.poll();
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
  }

  // Non-blocking drain — blocking receive would hang once the queue is empty.
  client.setBlocking(false);

  WorldState last_state;
  bool got_world = false;
  for (int i = 0; i < 500; ++i) {
    server.poll();
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

  if (!states.count(1) || states[1].move.tick() != 4 || !net::is_idle(states[1].move)) {
    std::cerr << "[server] smoke failed: expected last state IDLE tick=4\n";
    return 1;
  }

  const auto end = game.objects().front().position();
  std::cout << "[server] logic pos start=(" << start.x << "," << start.y << ") end=(" << end.x
            << "," << end.y << ")\n";

  if (end.x == start.x && end.y == start.y) {
    std::cerr << "[server] smoke failed: DynamicObject did not move\n";
    return 1;
  }

  if (!got_world) {
    std::cerr << "[server] smoke failed: no WorldState received by client\n";
    return 1;
  }

  const auto& p0 = last_state.players(0);
  const float dx = p0.position().x() - end.x;
  const float dy = p0.position().y() - end.y;
  if (dx * dx + dy * dy > 0.01f) {
    std::cerr << "[server] smoke failed: WorldState pos mismatch " << "wire=(" << p0.position().x()
              << "," << p0.position().y() << ") " << "logic=(" << end.x << "," << end.y << ")\n";
    return 1;
  }

  std::cout << "[server] WorldState ok tick=" << last_state.tick()
            << " players=" << last_state.players_size() << " p0=(" << p0.position().x() << ","
            << p0.position().y() << ")\n";
  std::cout << "BattleOfTheFlatland server — smoke ok (Move + GameLogic + WorldState)\n";
  return 0;
}

int run_loop(net::TcpServer& server,
             net::InboundQueue& inbound,
             net::OutboundQueue& outbound,
             std::unordered_map<ClientId, ClientInputState>& states,
             GameLogic& game) {
  std::cout << "[server] running on port " << kPort << " at " << kTickHz
            << " Hz (Ctrl+C to stop)\n";
  std::cout << "[server] Move → GameLogic.set_client_input → tick(" << kDt
            << ") → WorldState broadcast\n";

  std::uint64_t server_tick = 0;
  while (g_running) {
    const auto tick_start = std::chrono::steady_clock::now();

    server.poll();
    apply_inbound_move(inbound, states, game, true);
    game.tick(kDt);
    ++server_tick;
    enqueue_world_state(outbound, game, static_cast<std::uint32_t>(server_tick), server_tick);
    server.poll();

    if (server_tick % static_cast<std::uint64_t>(kTickHz) == 0) {
      const auto& objects = game.objects();
      std::cout << "[server] heartbeat tick=" << server_tick << " clients=" << server.client_count()
                << " entities=" << objects.size();
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

void print_usage(const char* argv0) {
  std::cout << "Usage: " << argv0 << " [--smoke]\n"
            << "  (default) long-running server loop\n"
            << "  --smoke   self-test then exit\n";
}

}  // namespace

int run_server(int argc, char** argv) {
  bool smoke = false;
  for (int i = 1; i < argc; ++i) {
    if (std::strcmp(argv[i], "--smoke") == 0) {
      smoke = true;
    } else if (std::strcmp(argv[i], "--help") == 0 || std::strcmp(argv[i], "-h") == 0) {
      print_usage(argv[0]);
      return 0;
    } else {
      std::cerr << "Unknown argument: " << argv[i] << '\n';
      print_usage(argv[0]);
      return 1;
    }
  }

  std::signal(SIGINT, on_signal);
  std::signal(SIGTERM, on_signal);

  net::InboundQueue inbound;
  net::OutboundQueue outbound;
  net::TcpServer server(inbound, outbound);
  std::unordered_map<ClientId, ClientInputState> input_states;
  GameLogic game;

  if (!server.listen(kPort)) {
    return 1;
  }

  const int code = smoke ? run_smoke(server, inbound, outbound, input_states, game)
                         : run_loop(server, inbound, outbound, input_states, game);

  server.stop();
  return code;
}

}  // namespace botf

int main(int argc, char** argv) {
  return botf::run_server(argc, argv);
}
