#pragma once

#include "net/notification_queue.hpp"
#include "net/types.hpp"

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>

#include <SFML/Network.hpp>

namespace botf::net {

using InboundQueue = NotificationQueue<InboundNotification>;
using OutboundQueue = NotificationQueue<OutboundNotification>;

// SFML TCP server: accepts clients, fills inbound queue, drains outbound queue.
class TcpServer {
 public:
  TcpServer(InboundQueue& inbound, OutboundQueue& outbound);
  ~TcpServer();

  TcpServer(const TcpServer&) = delete;
  TcpServer& operator=(const TcpServer&) = delete;

  // Bind and listen. Returns false on failure.
  bool listen(std::uint16_t port);

  void stop();

  // Accept / recv / send once. Call from the server loop.
  void poll();

  [[nodiscard]] bool is_listening() const { return m_listening; }
  [[nodiscard]] std::size_t client_count() const { return m_clients.size(); }
  [[nodiscard]] std::uint16_t port() const { return m_port; }

 private:
  struct Client {
    botf::ClientId id{};
    std::unique_ptr<sf::TcpSocket> socket;
    std::vector<std::uint8_t> recv_buffer;
    std::vector<std::uint8_t> send_buffer;
  };

  void accept_new_clients();
  void receive_from_clients();
  void dispatch_outbound();
  void flush_send_buffers();
  void drop_client(botf::ClientId id);

  InboundQueue& m_inbound;
  OutboundQueue& m_outbound;

  sf::TcpListener m_listener;
  sf::SocketSelector m_selector;
  std::unordered_map<botf::ClientId, Client> m_clients;

  botf::ClientId m_next_client_id = 1;
  std::uint16_t m_port = 0;
  bool m_listening = false;
};

}  // namespace botf::net
