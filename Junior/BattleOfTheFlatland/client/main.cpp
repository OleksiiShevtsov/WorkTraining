#include "app/app_rendering.h"
#include "envelope.pb.h"
#include "net/packet_framing.hpp"
#include "net/protocol.hpp"
#include "world_state.pb.h"

#include <array>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <optional>
#include <vector>

#include <SFML/Network.hpp>

namespace botf {
namespace {

constexpr std::uint16_t kPort = 54545;

bool send_move(sf::TcpSocket& socket, int dx, int dy, std::uint32_t tick, std::uint64_t sequence) {
  const auto envelope = net::make_move_envelope(dx, dy, tick, sequence);
  const auto packet = net::frame_payload(envelope);
  return socket.send(packet.data(), packet.size()) == sf::Socket::Status::Done;
}

bool try_connect(sf::TcpSocket& socket) {
  socket.setBlocking(true);
  const auto status = socket.connect(sf::IpAddress::LocalHost, kPort, sf::seconds(1));
  socket.setBlocking(false);
  return status == sf::Socket::Status::Done;
}

std::vector<app::SceneObject> scene_from_world_state(const WorldState& state) {
  std::vector<app::SceneObject> scene;
  scene.reserve(static_cast<std::size_t>(state.players_size()));
  for (const auto& player : state.players()) {
    scene.push_back(app::SceneObject{
        player.id(),
        common::Point{player.position().x(), player.position().y()},
        player.aim_angle(),
    });
  }
  return scene;
}

// Non-blocking drain: append socket bytes, keep latest WorldState.
void poll_inbound(sf::TcpSocket& socket,
                  std::vector<std::uint8_t>& recv_buffer,
                  std::optional<WorldState>& latest_world,
                  bool& net_ok) {
  while (net_ok) {
    std::array<char, 4096> chunk{};
    std::size_t received = 0;
    const auto status = socket.receive(chunk.data(), chunk.size(), received);

    if (status == sf::Socket::Status::Disconnected || status == sf::Socket::Status::Error) {
      std::cerr << "[client] receive failed — disconnecting network\n";
      net_ok = false;
      socket.disconnect();
      return;
    }
    if (status == sf::Socket::Status::NotReady || received == 0) {
      break;
    }
    if (status != sf::Socket::Status::Done) {
      break;
    }

    recv_buffer.insert(recv_buffer.end(), chunk.data(), chunk.data() + received);
  }

  while (auto payload = net::try_consume_frame(recv_buffer)) {
    auto envelope = net::parse_envelope(*payload);
    if (!envelope) {
      std::cerr << "[client] bad envelope from server\n";
      continue;
    }
    if (envelope->type() != MESSAGE_TYPE_WORLD_STATE) {
      continue;
    }
    auto state = net::parse_world_state_payload(envelope->payload());
    if (!state) {
      std::cerr << "[client] bad WorldState payload\n";
      continue;
    }
    latest_world = std::move(*state);
  }
}

int run_smoke_no_window() {
  app::InputHandler input;
  const auto sample = input.read_move_input();  // likely idle without display focus
  const auto move_packet = net::frame_payload(net::make_move_envelope(sample.dx, sample.dy, 1, 1));

  WorldState state;
  state.set_tick(42);
  auto* player = state.add_players();
  player->set_id(7);
  player->mutable_position()->set_x(100.f);
  player->mutable_position()->set_y(300.f);
  player->set_aim_angle(1.25f);

  const auto framed = net::frame_payload(net::make_world_state_envelope(state, /*sequence=*/1));

  std::vector<std::uint8_t> buffer(framed.begin(), framed.end());
  auto consumed = net::try_consume_frame(buffer);
  if (!consumed) {
    std::cerr << "[client] smoke failed: could not consume WorldState frame\n";
    return 1;
  }
  auto envelope = net::parse_envelope(*consumed);
  if (!envelope || envelope->type() != MESSAGE_TYPE_WORLD_STATE) {
    std::cerr << "[client] smoke failed: envelope type\n";
    return 1;
  }
  auto parsed = net::parse_world_state_payload(envelope->payload());
  if (!parsed || parsed->players_size() != 1) {
    std::cerr << "[client] smoke failed: WorldState parse\n";
    return 1;
  }

  const auto scene = scene_from_world_state(*parsed);
  if (scene.size() != 1 || scene[0].id != 7 || scene[0].position.x != 100.f ||
      scene[0].position.y != 300.f) {
    std::cerr << "[client] smoke failed: SceneObject mapping\n";
    return 1;
  }

  app::Player local_player;
  local_player.set_id(1);

  std::cout << "[client] smoke ok scene_id=" << scene[0].id << " move_packet=" << move_packet.size()
            << " world_packet=" << framed.size() << " tick=" << parsed->tick() << '\n';
  return 0;
}

void print_usage(const char* argv0) {
  std::cout << "Usage: " << argv0 << " [--smoke]\n"
            << "  (default) open SFML window, Move ↔ WorldState loop\n"
            << "  --smoke   no window; validate WorldState → SceneObject\n";
}

}  // namespace

int run_client(int argc, char** argv) {
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

  if (smoke) {
    return run_smoke_no_window();
  }

  app::AppRendering app;
  app.player().set_id(0);

  sf::TcpSocket socket;
  const bool connected = try_connect(socket);
  if (!connected) {
    std::cerr << "[client] server not available on " << kPort
              << " — UI only (Move/WorldState offline)\n";
  } else {
    std::cout << "[client] connected to server\n";
  }

  std::uint64_t sequence = 1;
  std::uint32_t tick = 1;
  app::MoveInput last_sent{};
  bool force_send = true;  // send first Move even if idle
  bool net_ok = connected;
  std::vector<std::uint8_t> recv_buffer;
  std::optional<WorldState> latest_world;

  while (app.is_window_open()) {
    const app::MoveInput input = app.input_handler().read_move_input();

    if (net_ok &&
        (force_send || input.dx != last_sent.dx || input.dy != last_sent.dy || (tick % 3u) == 0u)) {
      if (!send_move(socket, input.dx, input.dy, tick, sequence)) {
        std::cerr << "[client] send failed — disconnecting network\n";
        net_ok = false;
        socket.disconnect();
      } else {
        last_sent = input;
        force_send = false;
        ++sequence;
      }
    }
    ++tick;

    if (net_ok) {
      poll_inbound(socket, recv_buffer, latest_world, net_ok);
    }

    if (latest_world) {
      app.set_scene(scene_from_world_state(*latest_world));
    }

    app.draw();
  }

  if (net_ok) {
    socket.disconnect();
  }
  return 0;
}

}  // namespace botf

int main(int argc, char** argv) {
  return botf::run_client(argc, argv);
}
