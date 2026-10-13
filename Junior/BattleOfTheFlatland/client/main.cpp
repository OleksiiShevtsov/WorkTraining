#include "app/app_rendering.h"
#include "net/packet_framing.hpp"
#include "net/protocol.hpp"

#include <cstdint>
#include <cstring>
#include <iostream>

#include <SFML/Network.hpp>

namespace botf {
namespace {

constexpr std::uint16_t kPort = 54545;

bool send_move(sf::TcpSocket& socket,
               int dx,
               int dy,
               std::uint32_t tick,
               std::uint64_t sequence) {
  const auto envelope = net::make_move_envelope(dx, dy, tick, sequence);
  const auto packet = net::frame_payload(envelope);
  return socket.send(packet.data(), packet.size()) == sf::Socket::Done;
}

bool try_connect(sf::TcpSocket& socket) {
  socket.setBlocking(true);
  const auto status = socket.connect(sf::IpAddress::LocalHost, kPort, sf::seconds(1));
  socket.setBlocking(false);
  return status == sf::Socket::Done;
}

int run_smoke_no_window() {
  app::InputHandler input;
  const auto sample = input.read_move_input();  // likely idle without display focus
  const auto packet =
      net::frame_payload(net::make_move_envelope(sample.dx, sample.dy, 1, 1));

  app::Player player;
  player.set_id(1);

  app::SceneObject obj{player.id(), common::Point{100.f, 300.f}, 0.f};
  std::cout << "[client] smoke ok scene_id=" << obj.id
            << " packet_size=" << packet.size() << '\n';
  return 0;
}

void print_usage(const char* argv0) {
  std::cout << "Usage: " << argv0 << " [--smoke]\n"
            << "  (default) open SFML window, send Move to server\n"
            << "  --smoke   no window; validate app types + Move packet\n";
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
              << " — UI only (Move will not be sent)\n";
  } else {
    std::cout << "[client] connected to server\n";
  }

  std::uint64_t sequence = 1;
  std::uint32_t tick = 1;
  app::MoveInput last_sent{999, 999};  // force first send
  bool net_ok = connected;

  while (app.is_window_open()) {
    const app::MoveInput input = app.input_handler().read_move_input();

    if (net_ok && (input.dx != last_sent.dx || input.dy != last_sent.dy ||
                   (tick % 3u) == 0u)) {
      if (!send_move(socket, input.dx, input.dy, tick, sequence)) {
        std::cerr << "[client] send failed — disconnecting network\n";
        net_ok = false;
        socket.disconnect();
      } else {
        last_sent = input;
        ++sequence;
      }
    }
    ++tick;

    // Scene filled from WorldState later: app.set_scene(...);
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
