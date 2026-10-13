#include "net/tcp_server.hpp"

#include "net/packet_framing.hpp"

#include <array>
#include <iostream>
#include <utility>

namespace botf::net {
namespace {

void append_bytes(std::vector<std::uint8_t>& dst, const std::vector<std::uint8_t>& src) {
  dst.insert(dst.end(), src.begin(), src.end());
}

}  // namespace

TcpServer::TcpServer(InboundQueue& inbound, OutboundQueue& outbound)
    : inbound_(inbound), outbound_(outbound) {}

TcpServer::~TcpServer() {
  stop();
}

bool TcpServer::listen(std::uint16_t port) {
  stop();

  listener_.setBlocking(false);
  if (listener_.listen(port) != sf::Socket::Done) {
    std::cerr << "[net] failed to listen on port " << port << '\n';
    return false;
  }

  selector_.add(listener_);
  port_ = port;
  listening_ = true;
  std::cout << "[net] listening on port " << port_ << '\n';
  return true;
}

void TcpServer::stop() {
  if (!listening_ && clients_.empty()) {
    return;
  }

  for (auto& [id, client] : clients_) {
    (void)id;
    selector_.remove(*client.socket);
    client.socket->disconnect();
  }
  clients_.clear();

  selector_.remove(listener_);
  listener_.close();
  listening_ = false;
  port_ = 0;
}

void TcpServer::poll() {
  if (!listening_) {
    return;
  }

  dispatch_outbound();

  if (selector_.wait(sf::milliseconds(1))) {
    accept_new_clients();
    receive_from_clients();
  }

  flush_send_buffers();
}

void TcpServer::accept_new_clients() {
  if (!selector_.isReady(listener_)) {
    return;
  }

  auto socket = std::make_unique<sf::TcpSocket>();
  if (listener_.accept(*socket) != sf::Socket::Done) {
    return;
  }

  socket->setBlocking(false);
  const botf::ClientId id = next_client_id_++;
  selector_.add(*socket);

  Client client;
  client.id = id;
  client.socket = std::move(socket);
  clients_.emplace(id, std::move(client));

  std::cout << "[net] client connected id=" << id << " (total " << clients_.size() << ")\n";
}

void TcpServer::receive_from_clients() {
  std::vector<botf::ClientId> to_drop;

  for (auto& [id, client] : clients_) {
    if (!selector_.isReady(*client.socket)) {
      continue;
    }

    std::array<char, 4096> chunk{};
    std::size_t received = 0;
    const auto status = client.socket->receive(chunk.data(), chunk.size(), received);

    if (status == sf::Socket::Disconnected || status == sf::Socket::Error) {
      to_drop.push_back(id);
      continue;
    }
    if (status != sf::Socket::Done || received == 0) {
      continue;
    }

    client.recv_buffer.insert(client.recv_buffer.end(), chunk.data(), chunk.data() + received);

    while (auto payload = try_consume_frame(client.recv_buffer)) {
      inbound_.push(InboundNotification{id, std::move(*payload)});
    }
  }

  for (const botf::ClientId id : to_drop) {
    drop_client(id);
  }
}

void TcpServer::dispatch_outbound() {
  while (auto note = outbound_.try_pop()) {
    const auto framed = frame_payload(note->payload);

    if (!note->client_id.has_value()) {
      for (auto& [id, client] : clients_) {
        (void)id;
        append_bytes(client.send_buffer, framed);
      }
      continue;
    }

    const auto it = clients_.find(*note->client_id);
    if (it == clients_.end()) {
      continue;
    }
    append_bytes(it->second.send_buffer, framed);
  }
}

void TcpServer::flush_send_buffers() {
  std::vector<botf::ClientId> to_drop;

  for (auto& [id, client] : clients_) {
    while (!client.send_buffer.empty()) {
      std::size_t sent = 0;
      const auto status =
          client.socket->send(client.send_buffer.data(), client.send_buffer.size(), sent);

      if (status == sf::Socket::Disconnected || status == sf::Socket::Error) {
        to_drop.push_back(id);
        break;
      }

      if (sent > 0) {
        client.send_buffer.erase(client.send_buffer.begin(),
                                 client.send_buffer.begin() + static_cast<std::ptrdiff_t>(sent));
      }

      if (status == sf::Socket::Partial || status == sf::Socket::NotReady) {
        break;
      }
      if (status != sf::Socket::Done) {
        break;
      }
    }
  }

  for (const botf::ClientId id : to_drop) {
    drop_client(id);
  }
}

void TcpServer::drop_client(botf::ClientId id) {
  const auto it = clients_.find(id);
  if (it == clients_.end()) {
    return;
  }

  selector_.remove(*it->second.socket);
  it->second.socket->disconnect();
  clients_.erase(it);
  std::cout << "[net] client disconnected id=" << id << " (total " << clients_.size() << ")\n";
}

}  // namespace botf::net
