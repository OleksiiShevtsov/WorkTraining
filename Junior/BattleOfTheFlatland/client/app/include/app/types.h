#pragma once

#include "common/types.hpp"

namespace botf::app {

// Client-side view model for drawing (not server logic::DynamicObject).
struct SceneObject {
  common::ID id{};
  common::Point position{};
  common::Angle angle{};
};

struct MoveInput {
  int dx{0};
  int dy{0};
};

}  // namespace botf::app
