#include "app/player.h"

namespace botf::app {

Player::Player() : m_id{0} {}

Player::Player(common::ID id) : m_id{id} {}

common::ID Player::id() const {
  return m_id;
}

void Player::set_id(common::ID id) {
  m_id = id;
}

}  // namespace botf::app
