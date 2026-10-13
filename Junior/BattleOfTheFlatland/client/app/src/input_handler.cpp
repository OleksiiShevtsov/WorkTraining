#include "app/input_handler.h"

#include <SFML/Window/Keyboard.hpp>

namespace botf::app {

MoveInput InputHandler::read_move_input() const {
  MoveInput input{};

  const bool up =
      sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W);
  const bool down =
      sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S);
  const bool left =
      sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A);
  const bool right = sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
                     sf::Keyboard::isKeyPressed(sf::Keyboard::D);

  if (up) {
    input.dy += 1;
  }
  if (down) {
    input.dy -= 1;
  }
  if (right) {
    input.dx += 1;
  }
  if (left) {
    input.dx -= 1;
  }

  // Opposite keys cancel to 0; keep in {-1,0,1}.
  if (input.dx > 1) {
    input.dx = 1;
  }
  if (input.dx < -1) {
    input.dx = -1;
  }
  if (input.dy > 1) {
    input.dy = 1;
  }
  if (input.dy < -1) {
    input.dy = -1;
  }

  return input;
}

}  // namespace botf::app
