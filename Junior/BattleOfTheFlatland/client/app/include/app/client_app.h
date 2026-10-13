#pragma once

#include "app/app_rendering.h"
#include "net/tcp_client.hpp"
#include "world_state.pb.h"

#include <cstdint>
#include <optional>

namespace botf::app {

// Client runtime: window + input + TcpClient + WorldState → scene.
class ClientApp {
 public:
  ClientApp();

  ClientApp(const ClientApp&) = delete;
  ClientApp& operator=(const ClientApp&) = delete;

  // Open window and run until closed. Tries localhost:port.
  int run(std::uint16_t port = 54545);

  // No window: validate WorldState framing → SceneObject mapping.
  static int run_smoke();

 private:
  void apply_inbound();
  void maybe_send_move(const MoveInput& input, std::uint32_t tick);

  static std::vector<SceneObject> scene_from_world_state(const WorldState& state);

  net::ClientInboundQueue m_inbound;
  net::OutboundQueue m_outbound;
  net::TcpClient m_net;
  AppRendering m_rendering;

  std::uint64_t m_sequence = 1;
  MoveInput m_last_sent{};
  bool m_force_send = true;
  std::optional<WorldState> m_latest_world;
};

}  // namespace botf::app
