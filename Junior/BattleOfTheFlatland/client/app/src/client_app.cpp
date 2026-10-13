#include "app/client_app.h"

#include "envelope.pb.h"
#include "net/packet_framing.hpp"
#include "net/protocol.hpp"

#include <iostream>
#include <vector>

#include <SFML/Network.hpp>

namespace botf::app {

ClientApp::ClientApp() : m_net(m_inbound, m_outbound) {}

std::vector<SceneObject> ClientApp::sceneFromWorldState(const WorldState& state) {
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

void ClientApp::applyInbound() {
  while (auto note = m_inbound.tryPop()) {
    auto envelope = net::parseEnvelope(note->payload);
    if (!envelope) {
      std::cerr << "[client] bad envelope from server\n";
      continue;
    }
    if (envelope->type() != MESSAGE_TYPE_WORLD_STATE) {
      continue;
    }
    auto state = net::parseWorldStatePayload(envelope->payload());
    if (!state) {
      std::cerr << "[client] bad WorldState payload\n";
      continue;
    }
    m_latestWorld = std::move(*state);
  }
}

void ClientApp::maybeSendMove(const MoveInput& input, std::uint32_t tick) {
  if (!m_net.isConnected()) {
    return;
  }
  if (!(m_forceSend || input.dx != m_lastSent.dx || input.dy != m_lastSent.dy ||
        (tick % 3u) == 0u)) {
    return;
  }

  m_outbound.push(net::OutboundNotification{
      std::nullopt,
      net::makeMoveEnvelope(input.dx, input.dy, tick, m_sequence),
  });
  m_lastSent = input;
  m_forceSend = false;
  ++m_sequence;
}

int ClientApp::run(std::uint16_t port) {
  m_rendering.player().setId(0);

  if (!m_net.connect(sf::IpAddress::LocalHost, port, sf::seconds(1))) {
    std::cerr << "[client] server not available on " << port
              << " — UI only (Move/WorldState offline)\n";
  } else {
    std::cout << "[client] connected to server\n";
  }

  std::uint32_t tick = 1;
  while (m_rendering.isWindowOpen()) {
    const MoveInput input = m_rendering.inputHandler().readMoveInput();
    maybeSendMove(input, tick);
    ++tick;

    if (m_net.isConnected()) {
      m_net.poll();
      if (!m_net.isConnected()) {
        std::cerr << "[client] disconnected from server\n";
      }
    }

    applyInbound();

    if (m_latestWorld) {
      m_rendering.setScene(sceneFromWorldState(*m_latestWorld));
    }

    m_rendering.draw();
  }

  m_net.disconnect();
  return 0;
}

int ClientApp::runSmoke() {
  InputHandler input;
  const auto sample = input.readMoveInput();
  const auto movePacket = net::framePayload(net::makeMoveEnvelope(sample.dx, sample.dy, 1, 1));

  WorldState state;
  state.set_tick(42);
  auto* player = state.add_players();
  player->set_id(7);
  player->mutable_position()->set_x(100.f);
  player->mutable_position()->set_y(300.f);
  player->set_aim_angle(1.25f);

  const auto framed = net::framePayload(net::makeWorldStateEnvelope(state, /*sequence=*/1));

  std::vector<std::uint8_t> buffer(framed.begin(), framed.end());
  auto consumed = net::tryConsumeFrame(buffer);
  if (!consumed) {
    std::cerr << "[client] smoke failed: could not consume WorldState frame\n";
    return 1;
  }
  auto envelope = net::parseEnvelope(*consumed);
  if (!envelope || envelope->type() != MESSAGE_TYPE_WORLD_STATE) {
    std::cerr << "[client] smoke failed: envelope type\n";
    return 1;
  }
  auto parsed = net::parseWorldStatePayload(envelope->payload());
  if (!parsed || parsed->players_size() != 1) {
    std::cerr << "[client] smoke failed: WorldState parse\n";
    return 1;
  }

  const auto scene = sceneFromWorldState(*parsed);
  if (scene.size() != 1 || scene[0].id != 7 || scene[0].position.x != 100.f ||
      scene[0].position.y != 300.f) {
    std::cerr << "[client] smoke failed: SceneObject mapping\n";
    return 1;
  }

  Player localPlayer;
  localPlayer.setId(1);

  std::cout << "[client] smoke ok sceneId=" << scene[0].id << " movePacket=" << movePacket.size()
            << " worldPacket=" << framed.size() << " tick=" << parsed->tick() << '\n';
  return 0;
}

}  // namespace botf::app
