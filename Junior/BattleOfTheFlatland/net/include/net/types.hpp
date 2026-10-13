#pragma once

#include "common/types.hpp"

#include <optional>
#include <string>

namespace botf::net {

// Client → server (after length-prefix is stripped).
// `payload` is typically a serialized botf.Envelope.
struct InboundNotification {
  botf::ClientId client_id{};
  std::string payload;
};

// Server → client(s).
// `client_id == nullopt` means broadcast to every connected client.
struct OutboundNotification {
  std::optional<botf::ClientId> client_id;
  std::string payload;
};

}  // namespace botf::net
