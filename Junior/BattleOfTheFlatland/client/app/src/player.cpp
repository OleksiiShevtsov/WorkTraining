#include "app/player.h"

namespace botf::app {

Player::Player() : id_{0} {}

Player::Player(common::ID id) : id_{id} {}

common::ID Player::id() const {
  return id_;
}

void Player::set_id(common::ID id) {
  id_ = id;
}

}  // namespace botf::app
