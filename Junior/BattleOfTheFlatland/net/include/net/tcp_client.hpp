#pragma once

#include "net/queues.hpp"

#include <cstdint>
#include <string>
#include <vector>

#include <SFML/Network.hpp>

namespace botf::net {

// SFML TCP client: connect, drain outbound queue, fill inbound queue (framed payloads).
class TcpClient {
 public:
  TcpClient(ClientInboundQueue& inbound, OutboundQueue& outbound);
  ~TcpClient();

  TcpClient(const TcpClient&) = delete;
  TcpClient& operator=(const TcpClient&) = delete;

  // Blocking connect, then switches socket to non-blocking.
  bool connect(sf::IpAddress host, std::uint16_t port, sf::Time timeout = sf::seconds(1));

  void disconnect();

  // Send queued outbound + recv into inbound. Call from the client frame loop.
  void poll();

  [[nodiscard]] bool is_connected() const { return m_connected; }

 private:
  void dispatch_outbound();
  void receive_inbound();
  void flush_send_buffer();

  ClientInboundQueue& m_inbound;
  OutboundQueue& m_outbound;

  sf::TcpSocket m_socket;
  std::vector<std::uint8_t> m_recv_buffer;
  std::vector<std::uint8_t> m_send_buffer;
  bool m_connected = false;
};

}  // namespace botf::net
