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
    : m_inbound(inbound), m_outbound(outbound) {}

TcpServer::~TcpServer() {
  stop();
}

bool TcpServer::listen(std::uint16_t port) {
  stop();

  m_listener.setBlocking(false);
  if (m_listener.listen(port) != sf::Socket::Status::Done) {
    std::cerr << "[net] failed to listen on port " << port << '\n';
    return false;
  }

  m_selector.add(m_listener);
  m_port = port;
  m_listening = true;
  std::cout << "[net] listening on port " << m_port << '\n';
  return true;
}

void TcpServer::stop() {
  if (!m_listening && m_clients.empty()) {
    return;
  }

  for (auto& [id, client] : m_clients) {
    (void)id;
    m_selector.remove(*client.socket);
    client.socket->disconnect();
  }
  m_clients.clear();

  m_selector.remove(m_listener);
  m_listener.close();
  m_listening = false;
  m_port = 0;
}

void TcpServer::poll() {
  if (!m_listening) {
    return;
  }

  dispatch_outbound();

  if (m_selector.wait(sf::milliseconds(1))) {
    accept_new_clients();
    receive_from_clients();
  }

  flush_send_buffers();
}

void TcpServer::accept_new_clients() {
  if (!m_selector.isReady(m_listener)) {
    return;
  }

  auto socket = std::make_unique<sf::TcpSocket>();
  if (m_listener.accept(*socket) != sf::Socket::Status::Done) {
    return;
  }

  socket->setBlocking(false);
  const botf::ClientId id = m_next_client_id++;
  m_selector.add(*socket);

  Client client;
  client.id = id;
  client.socket = std::move(socket);
  m_clients.emplace(id, std::move(client));

  std::cout << "[net] client connected id=" << id << " (total " << m_clients.size() << ")\n";
}

void TcpServer::receive_from_clients() {
  std::vector<botf::ClientId> to_drop;

  for (auto& [id, client] : m_clients) {
    if (!m_selector.isReady(*client.socket)) {
      continue;
    }

    std::array<char, 4096> chunk{};
    std::size_t received = 0;
    const auto status = client.socket->receive(chunk.data(), chunk.size(), received);

    if (status == sf::Socket::Status::Disconnected || status == sf::Socket::Status::Error) {
      to_drop.push_back(id);
      continue;
    }
    if (status != sf::Socket::Status::Done || received == 0) {
      continue;
    }

    client.recv_buffer.insert(client.recv_buffer.end(), chunk.data(), chunk.data() + received);

    while (auto payload = try_consume_frame(client.recv_buffer)) {
      m_inbound.push(InboundNotification{id, std::move(*payload)});
    }
  }

  for (const botf::ClientId id : to_drop) {
    drop_client(id);
  }
}

void TcpServer::dispatch_outbound() {
  while (auto note = m_outbound.try_pop()) {
    const auto framed = frame_payload(note->payload);

    if (!note->client_id.has_value()) {
      for (auto& [id, client] : m_clients) {
        (void)id;
        append_bytes(client.send_buffer, framed);
      }
      continue;
    }

    const auto it = m_clients.find(*note->client_id);
    if (it == m_clients.end()) {
      continue;
    }
    append_bytes(it->second.send_buffer, framed);
  }
}

void TcpServer::flush_send_buffers() {
  std::vector<botf::ClientId> to_drop;

  for (auto& [id, client] : m_clients) {
    while (!client.send_buffer.empty()) {
      std::size_t sent = 0;
      const auto status =
          client.socket->send(client.send_buffer.data(), client.send_buffer.size(), sent);

      if (status == sf::Socket::Status::Disconnected || status == sf::Socket::Status::Error) {
        to_drop.push_back(id);
        break;
      }

      if (sent > 0) {
        client.send_buffer.erase(client.send_buffer.begin(),
                                 client.send_buffer.begin() + static_cast<std::ptrdiff_t>(sent));
      }

      if (status == sf::Socket::Status::Partial || status == sf::Socket::Status::NotReady) {
        break;
      }
      if (status != sf::Socket::Status::Done) {
        break;
      }
    }
  }

  for (const botf::ClientId id : to_drop) {
    drop_client(id);
  }
}

void TcpServer::drop_client(botf::ClientId id) {
  const auto it = m_clients.find(id);
  if (it == m_clients.end()) {
    return;
  }

  m_selector.remove(*it->second.socket);
  it->second.socket->disconnect();
  m_clients.erase(it);
  std::cout << "[net] client disconnected id=" << id << " (total " << m_clients.size() << ")\n";
}

}  // namespace botf::net
