#include "engine/server_smoke.h"

#include "engine/server_engine.h"
#include "envelope.pb.h"
#include "net/packet_framing.hpp"
#include "net/protocol.hpp"
#include "world_state.pb.h"

#include <array>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include <SFML/Network.hpp>

namespace botf {
namespace {

constexpr float kDt = 1.0f / 20.0f;

bool sendFramed(sf::TcpSocket& socket, const std::string& envelopeBytes) {
  const auto packet = net::framePayload(envelopeBytes);
  return socket.send(packet.data(), packet.size()) == sf::Socket::Status::Done;
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

int runServerSmoke(std::uint16_t port) {
  ServerEngine engine;
  if (!engine.startListen(port)) {
    return 1;
  }

  sf::TcpSocket client;
  client.setBlocking(true);
  if (client.connect(sf::IpAddress::LocalHost, port, sf::seconds(2)) != sf::Socket::Status::Done) {
    std::cerr << "[server] smoke client failed to connect\n";
    engine.shutdown();
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

  const auto start = engine.game().objects().front().position();
  std::vector<std::uint8_t> recvBuffer;

  for (const auto& sample : samples) {
    if (!sendFramed(client, net::makeMoveEnvelope(sample.dx, sample.dy, sample.tick, sample.seq))) {
      std::cerr << "[server] smoke client failed to send Move\n";
      engine.shutdown();
      return 1;
    }

    bool got = false;
    for (int i = 0; i < 100 && !got; ++i) {
      engine.pollNetwork();
      engine.applyInboundMoves(true);
      const auto move = engine.lastMove(1);
      if (move && move->tick() == sample.tick) {
        got = true;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
    if (!got) {
      std::cerr << "[server] smoke failed waiting for Move tick=" << sample.tick << '\n';
      engine.shutdown();
      return 1;
    }

    for (int j = 0; j < 15; ++j) {
      engine.advanceSim(kDt);
      engine.pollNetwork();
    }
  }

  // Last advanceSim already enqueued WorldState; poll to drain outbound.
  for (int i = 0; i < 20; ++i) {
    engine.pollNetwork();
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
  }

  client.setBlocking(false);

  WorldState lastState;
  bool gotWorld = false;
  for (int i = 0; i < 500; ++i) {
    engine.pollNetwork();
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

  const auto last = engine.lastMove(1);
  if (!last || last->tick() != 4 || !net::isIdle(*last)) {
    std::cerr << "[server] smoke failed: expected last state IDLE tick=4\n";
    engine.shutdown();
    return 1;
  }

  const auto end = engine.game().objects().front().position();
  std::cout << "[server] logic pos start=(" << start.x << "," << start.y << ") end=(" << end.x
            << "," << end.y << ")\n";

  if (end.x == start.x && end.y == start.y) {
    std::cerr << "[server] smoke failed: DynamicObject did not move\n";
    engine.shutdown();
    return 1;
  }

  if (!gotWorld) {
    std::cerr << "[server] smoke failed: no WorldState received by client\n";
    engine.shutdown();
    return 1;
  }

  const auto& p0 = lastState.players(0);
  const float dx = p0.position().x() - end.x;
  const float dy = p0.position().y() - end.y;
  if (dx * dx + dy * dy > 0.01f) {
    std::cerr << "[server] smoke failed: WorldState pos mismatch " << "wire=(" << p0.position().x()
              << "," << p0.position().y() << ") " << "logic=(" << end.x << "," << end.y << ")\n";
    engine.shutdown();
    return 1;
  }

  std::cout << "[server] WorldState ok tick=" << lastState.tick()
            << " players=" << lastState.players_size() << " p0=(" << p0.position().x() << ","
            << p0.position().y() << ")\n";
  std::cout << "BattleOfTheFlatland server — smoke ok (Move + GameLogic + WorldState)\n";
  engine.shutdown();
  return 0;
}

}  // namespace botf
