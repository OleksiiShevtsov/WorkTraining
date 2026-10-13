#pragma once

#include "common/types.hpp"

namespace botf::app {

class Player {
 public:
  Player();
  explicit Player(common::ID id);

  Player(const Player&) = delete;
  Player& operator=(const Player&) = delete;

  [[nodiscard]] common::ID id() const;
  void set_id(common::ID id);

 private:
  common::ID id_{0};
};

}  // namespace botf::app
