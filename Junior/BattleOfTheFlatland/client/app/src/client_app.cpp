#include "app/client_app.h"

#include "envelope.pb.h"
#include "net/protocol.hpp"

#include <iostream>

#include <SFML/Network.hpp>

botf::app::ClientApp::ClientApp() : m_net(m_inbound, m_outbound) {}

std::vector<botf::app::SceneObject> botf::app::sceneFromWorldState(const WorldState& state) {
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

void botf::app::ClientApp::applyInbound() {
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

void botf::app::ClientApp::maybeSendMove(const MoveInput& input, std::uint32_t tick) {
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

int botf::app::ClientApp::run(std::uint16_t port) {
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
