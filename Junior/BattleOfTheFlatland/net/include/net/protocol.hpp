#pragma once

#include "envelope.pb.h"
#include "move.pb.h"

#include <algorithm>
#include <cstdint>
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

}  // namespace botf::net
