#pragma once

#include "logic/game_logic.h"
#include "move.pb.h"
#include "net/tcp_server.hpp"

#include <cstdint>
#include <unordered_map>

namespace botf {

// Server runtime: TCP + GameLogic tick + WorldState broadcast.
// main should only parse args and call run() / run_smoke().
class ServerEngine {
 public:
  ServerEngine();
  ~ServerEngine();

  ServerEngine(const ServerEngine&) = delete;
  ServerEngine& operator=(const ServerEngine&) = delete;

  // Listen and run the 20 Hz loop until signal / stop.
  int run(std::uint16_t port = 54545);

  // Self-test: Move samples → tick → WorldState round-trip, then exit.
  int run_smoke(std::uint16_t port = 54545);

  void request_stop();

 private:
  struct ClientInputState {
    Move move{};
    std::uint64_t sequence = 0;
  };

  bool start_listen(std::uint16_t port);
  void shutdown();

  void ensure_client_bound(ClientId client_id);
  void apply_inbound_move(bool verbose);
  void enqueue_world_state(std::uint32_t tick, std::uint64_t sequence);
  int run_loop();

  net::InboundQueue m_inbound;
  net::OutboundQueue m_outbound;
  net::TcpServer m_server;
  GameLogic m_game;
  std::unordered_map<ClientId, ClientInputState> m_input_states;
  bool m_running = false;
};

}  // namespace botf
