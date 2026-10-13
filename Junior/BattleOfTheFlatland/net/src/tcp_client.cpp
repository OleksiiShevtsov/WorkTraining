#include "net/tcp_client.hpp"

#include "net/packet_framing.hpp"

#include <array>
#include <iostream>

namespace botf::net {
namespace {

void appendBytes(std::vector<std::uint8_t>& dst, const std::vector<std::uint8_t>& src) {
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
  m_recvBuffer.clear();
  m_sendBuffer.clear();
  return true;
}

void TcpClient::disconnect() {
  if (!m_connected && m_sendBuffer.empty() && m_recvBuffer.empty()) {
    m_socket.disconnect();
    return;
  }

  m_socket.disconnect();
  m_connected = false;
  m_recvBuffer.clear();
  m_sendBuffer.clear();
}

void TcpClient::poll() {
  if (!m_connected) {
    return;
  }

  dispatchOutbound();
  receiveInbound();
  flushSendBuffer();
}

void TcpClient::dispatchOutbound() {
  while (auto note = m_outbound.tryPop()) {
    // Client ignores targeting; every outbound goes to the connected server.
    appendBytes(m_sendBuffer, framePayload(note->payload));
  }
}

void TcpClient::receiveInbound() {
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

    m_recvBuffer.insert(m_recvBuffer.end(), chunk.data(), chunk.data() + received);
  }

  while (auto payload = tryConsumeFrame(m_recvBuffer)) {
    m_inbound.push(ClientInboundNotification{std::move(*payload)});
  }
}

void TcpClient::flushSendBuffer() {
  while (m_connected && !m_sendBuffer.empty()) {
    std::size_t sent = 0;
    const auto status = m_socket.send(m_sendBuffer.data(), m_sendBuffer.size(), sent);

    if (status == sf::Socket::Status::Disconnected || status == sf::Socket::Status::Error) {
      std::cerr << "[net] client send failed — disconnecting\n";
      disconnect();
      return;
    }

    if (sent > 0) {
      m_sendBuffer.erase(m_sendBuffer.begin(),
                         m_sendBuffer.begin() + static_cast<std::ptrdiff_t>(sent));
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
