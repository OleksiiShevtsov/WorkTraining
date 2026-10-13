#include "app/app_rendering.h"

#include "common/global_settings.hpp"

namespace botf::app {

AppRendering::AppRendering()
    : window_{std::make_shared<sf::RenderWindow>(
          sf::VideoMode(static_cast<unsigned int>(common::GlobalSettings::sizeWindowX),
                        static_cast<unsigned int>(common::GlobalSettings::sizeWindowY)),
          "Battle Of The Flatland")} {
  window_->setFramerateLimit(60);
}

Player& AppRendering::player() {
  return player_;
}

const Player& AppRendering::player() const {
  return player_;
}

InputHandler& AppRendering::input_handler() {
  return input_handler_;
}

const InputHandler& AppRendering::input_handler() const {
  return input_handler_;
}

void AppRendering::clear_scene() {
  scene_.clear();
}

void AppRendering::set_scene_object(common::ID id, common::Point position, common::Angle angle) {
  for (auto& obj : scene_) {
    if (obj.id == id) {
      obj.position = position;
      obj.angle = angle;
      return;
    }
  }
  scene_.push_back(SceneObject{id, position, angle});
}

void AppRendering::set_scene(std::vector<SceneObject> objects) {
  scene_ = std::move(objects);
}

bool AppRendering::is_window_open() const {
  return window_ && window_->isOpen();
}

void AppRendering::draw() {
  if (!window_) {
    return;
  }

  sf::Event event{};
  while (window_->pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      window_->close();
    }
  }

  window_->clear(sf::Color(25, 25, 25));
  draw_map();
  draw_scene_objects();
  window_->display();

  // Fresh scene each frame; caller fills from WorldState before draw().
  scene_.clear();
}

void AppRendering::draw_map() {
  // Map rendering later.
}

void AppRendering::draw_scene_objects() {
  for (const auto& el : scene_) {
    sf::CircleShape circle(10.f);
    circle.setFillColor(sf::Color(120, 20, 20));
    circle.setOrigin(10.f, 10.f);
    circle.setPosition(el.position.x, el.position.y);
    window_->draw(circle);
  }
}

}  // namespace botf::app
