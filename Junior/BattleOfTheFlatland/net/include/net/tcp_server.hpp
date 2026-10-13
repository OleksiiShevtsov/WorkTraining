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

  [[nodiscard]] bool is_listening() const { return listening_; }
  [[nodiscard]] std::size_t client_count() const { return clients_.size(); }
  [[nodiscard]] std::uint16_t port() const { return port_; }

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

  InboundQueue& inbound_;
  OutboundQueue& outbound_;

  sf::TcpListener listener_;
  sf::SocketSelector selector_;
  std::unordered_map<botf::ClientId, Client> clients_;

  botf::ClientId next_client_id_ = 1;
  std::uint16_t port_ = 0;
  bool listening_ = false;
};

}  // namespace botf::net
