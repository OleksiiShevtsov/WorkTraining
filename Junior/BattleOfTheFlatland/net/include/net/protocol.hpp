#pragma once

#include "envelope.pb.h"
#include "move.pb.h"
#include "world_state.pb.h"

#include <algorithm>
#include <cstdint>
#include <optional>
#include <string>

namespace botf::net {

inline int clampAxis(int value) {
  return std::clamp(value, -1, 1);
}

// Normalize movement state: each axis in {-1, 0, 1}.
inline botf::Move normalizeMove(botf::Move move) {
  move.set_dx(clampAxis(move.dx()));
  move.set_dy(clampAxis(move.dy()));
  return move;
}

inline bool isIdle(const botf::Move& move) {
  return move.dx() == 0 && move.dy() == 0;
}

inline std::string toString(const botf::Move& move) {
  const botf::Move n = normalizeMove(move);
  if (isIdle(n)) {
    return "IDLE";
  }

  std::string out;
  if (n.dy() > 0) {
    out += "UP";
  } else if (n.dy() < 0) {
    out += "DOWN";
  }
  if (n.dx() > 0) {
    if (!out.empty()) {
      out += "+";
    }
    out += "RIGHT";
  } else if (n.dx() < 0) {
    if (!out.empty()) {
      out += "+";
    }
    out += "LEFT";
  }
  return out;
}

// Serialized Envelope bytes (without length prefix).
inline std::string makeMoveEnvelope(int dx, int dy, std::uint32_t tick, std::uint64_t sequence) {
  botf::Move move;
  move.set_dx(clampAxis(dx));
  move.set_dy(clampAxis(dy));
  move.set_tick(tick);

  botf::Envelope envelope;
  envelope.set_type(botf::MESSAGE_TYPE_MOVE);
  envelope.set_sequence(sequence);
  envelope.set_payload(move.SerializeAsString());
  return envelope.SerializeAsString();
}

// Serialized Envelope(MESSAGE_TYPE_WORLD_STATE) bytes (without length prefix).
inline std::string makeWorldStateEnvelope(const botf::WorldState& state, std::uint64_t sequence) {
  botf::Envelope envelope;
  envelope.set_type(botf::MESSAGE_TYPE_WORLD_STATE);
  envelope.set_sequence(sequence);
  envelope.set_payload(state.SerializeAsString());
  return envelope.SerializeAsString();
}

inline std::optional<botf::Envelope> parseEnvelope(const std::string& bytes) {
  botf::Envelope envelope;
  if (!envelope.ParseFromString(bytes)) {
    return std::nullopt;
  }
  return envelope;
}

inline std::optional<botf::WorldState> parseWorldStatePayload(const std::string& payload) {
  botf::WorldState state;
  if (!state.ParseFromString(payload)) {
    return std::nullopt;
  }
  return state;
}

}  // namespace botf::net
