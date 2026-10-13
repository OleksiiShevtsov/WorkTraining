#include "net/tcp_client.hpp"

#include "net/packet_framing.hpp"

#include <array>
#include <iostream>

namespace botf::net {
namespace {

void append_bytes(std::vector<std::uint8_t>& dst, const std::vector<std::uint8_t>& src) {
  dst.insert(dst.end(), src.begin(), src.end());
}

}  // namespace

TcpClient::TcpClient(ClientInboundQueue& inbound, OutboundQueue& outbound)
    : m_inbound(inbound), m_outbound(outbound) {}

TcpClient::~TcpClient() {
  disconnect();
}

bool TcpClient::connect(sf::IpAddress host, std::uint16_t port, sf::Time timeout) {
  disconnect();

  m_socket.setBlocking(true);
  const auto status = m_socket.connect(host, port, timeout);
  if (status != sf::Socket::Status::Done) {
    m_socket.disconnect();
    return false;
  }

  m_socket.setBlocking(false);
  m_connected = true;
  m_recv_buffer.clear();
  m_send_buffer.clear();
  return true;
}

void TcpClient::disconnect() {
  if (!m_connected && m_send_buffer.empty() && m_recv_buffer.empty()) {
    m_socket.disconnect();
    return;
  }

  m_socket.disconnect();
  m_connected = false;
  m_recv_buffer.clear();
  m_send_buffer.clear();
}

void TcpClient::poll() {
  if (!m_connected) {
    return;
  }

  dispatch_outbound();
  receive_inbound();
  flush_send_buffer();
}

void TcpClient::dispatch_outbound() {
  while (auto note = m_outbound.try_pop()) {
    // Client ignores targeting; every outbound goes to the connected server.
    append_bytes(m_send_buffer, frame_payload(note->payload));
  }
}

void TcpClient::receive_inbound() {
  while (m_connected) {
    std::array<char, 4096> chunk{};
    std::size_t received = 0;
    const auto status = m_socket.receive(chunk.data(), chunk.size(), received);

    if (status == sf::Socket::Status::Disconnected || status == sf::Socket::Status::Error) {
      std::cerr << "[net] client disconnected from server\n";
      disconnect();
      return;
    }
    if (status == sf::Socket::Status::NotReady || received == 0) {
      break;
    }
    if (status != sf::Socket::Status::Done) {
      break;
    }

    m_recv_buffer.insert(m_recv_buffer.end(), chunk.data(), chunk.data() + received);
  }

  while (auto payload = try_consume_frame(m_recv_buffer)) {
    m_inbound.push(ClientInboundNotification{std::move(*payload)});
  }
}

void TcpClient::flush_send_buffer() {
  while (m_connected && !m_send_buffer.empty()) {
    std::size_t sent = 0;
    const auto status = m_socket.send(m_send_buffer.data(), m_send_buffer.size(), sent);

    if (status == sf::Socket::Status::Disconnected || status == sf::Socket::Status::Error) {
      std::cerr << "[net] client send failed — disconnecting\n";
      disconnect();
      return;
    }

    if (sent > 0) {
      m_send_buffer.erase(m_send_buffer.begin(),
                          m_send_buffer.begin() + static_cast<std::ptrdiff_t>(sent));
    }

    if (status == sf::Socket::Status::Partial || status == sf::Socket::Status::NotReady) {
      break;
    }
    if (status != sf::Socket::Status::Done) {
      break;
    }
  }
}

}  // namespace botf::net
