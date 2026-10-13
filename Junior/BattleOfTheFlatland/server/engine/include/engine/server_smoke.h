#pragma once

#include <cstdint>

namespace botf {

// Integration self-test (not part of the long-running server loop):
// Move samples → GameLogic ticks → WorldState round-trip, then exit.
int runServerSmoke(std::uint16_t port = 54545);

}  // namespace botf
