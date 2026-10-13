#pragma once

#include "envelope.pb.h"
#include "move.pb.h"
#include "world_state.pb.h"

#include <algorithm>
#include <cstdint>
#include <optional>
#include <string>

namespace botf::net {

inline int clamp_axis(int value) {
  return std::clamp(value, -1, 1);
}

// Normalize movement state: each axis in {-1, 0, 1}.
inline botf::Move normalize_move(botf::Move move) {
  move.set_dx(clamp_axis(move.dx()));
  move.set_dy(clamp_axis(move.dy()));
  return move;
}

inline bool is_idle(const botf::Move& move) {
  return move.dx() == 0 && move.dy() == 0;
}

inline std::string to_string(const botf::Move& move) {
  const botf::Move n = normalize_move(move);
  if (is_idle(n)) {
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
inline std::string make_move_envelope(int dx, int dy, std::uint32_t tick, std::uint64_t sequence) {
  botf::Move move;
  move.set_dx(clamp_axis(dx));
  move.set_dy(clamp_axis(dy));
  move.set_tick(tick);

  botf::Envelope envelope;
  envelope.set_type(botf::MESSAGE_TYPE_MOVE);
  envelope.set_sequence(sequence);
  envelope.set_payload(move.SerializeAsString());
  return envelope.SerializeAsString();
}

// Serialized Envelope(MESSAGE_TYPE_WORLD_STATE) bytes (without length prefix).
inline std::string make_world_state_envelope(const botf::WorldState& state,
                                             std::uint64_t sequence) {
  botf::Envelope envelope;
  envelope.set_type(botf::MESSAGE_TYPE_WORLD_STATE);
  envelope.set_sequence(sequence);
  envelope.set_payload(state.SerializeAsString());
  return envelope.SerializeAsString();
}

inline std::optional<botf::Envelope> parse_envelope(const std::string& bytes) {
  botf::Envelope envelope;
  if (!envelope.ParseFromString(bytes)) {
    return std::nullopt;
  }
  return envelope;
}

inline std::optional<botf::WorldState> parse_world_state_payload(const std::string& payload) {
  botf::WorldState state;
  if (!state.ParseFromString(payload)) {
    return std::nullopt;
  }
  return state;
}

}  // namespace botf::net
