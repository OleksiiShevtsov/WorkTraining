#include "app/client_app.h"

#include "envelope.pb.h"
#include "net/packet_framing.hpp"
#include "net/protocol.hpp"

#include <iostream>
#include <vector>

#include <SFML/Network.hpp>

namespace botf::app {

ClientApp::ClientApp() : m_net(m_inbound, m_outbound) {}

std::vector<SceneObject> ClientApp::scene_from_world_state(const WorldState& state) {
  std::vector<SceneObject> scene;
  scene.reserve(static_cast<std::size_t>(state.players_size()));
  for (const auto& player : state.players()) {
    scene.push_back(SceneObject{
        player.id(),
        common::Point{player.position().x(), player.position().y()},
        player.aim_angle(),
    });
  }
  return scene;
}

void ClientApp::apply_inbound() {
  while (auto note = m_inbound.try_pop()) {
    auto envelope = net::parse_envelope(note->payload);
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
    m_latest_world = std::move(*state);
  }
}

void ClientApp::maybe_send_move(const MoveInput& input, std::uint32_t tick) {
  if (!m_net.is_connected()) {
    return;
  }
  if (!(m_force_send || input.dx != m_last_sent.dx || input.dy != m_last_sent.dy ||
        (tick % 3u) == 0u)) {
    return;
  }

  m_outbound.push(net::OutboundNotification{
      std::nullopt,
      net::make_move_envelope(input.dx, input.dy, tick, m_sequence),
  });
  m_last_sent = input;
  m_force_send = false;
  ++m_sequence;
}

int ClientApp::run(std::uint16_t port) {
  m_rendering.player().set_id(0);

  if (!m_net.connect(sf::IpAddress::LocalHost, port, sf::seconds(1))) {
    std::cerr << "[client] server not available on " << port
              << " — UI only (Move/WorldState offline)\n";
  } else {
    std::cout << "[client] connected to server\n";
  }

  std::uint32_t tick = 1;
  while (m_rendering.is_window_open()) {
    const MoveInput input = m_rendering.input_handler().read_move_input();
    maybe_send_move(input, tick);
    ++tick;

    if (m_net.is_connected()) {
      m_net.poll();
      if (!m_net.is_connected()) {
        std::cerr << "[client] disconnected from server\n";
      }
    }

    apply_inbound();

    if (m_latest_world) {
      m_rendering.set_scene(scene_from_world_state(*m_latest_world));
    }

    m_rendering.draw();
  }

  m_net.disconnect();
  return 0;
}

int ClientApp::run_smoke() {
  InputHandler input;
  const auto sample = input.read_move_input();
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

  Player local_player;
  local_player.set_id(1);

  std::cout << "[client] smoke ok scene_id=" << scene[0].id << " move_packet=" << move_packet.size()
            << " world_packet=" << framed.size() << " tick=" << parsed->tick() << '\n';
  return 0;
}

}  // namespace botf::app
