#include "app/app_rendering.h"

#include "common/global_settings.hpp"

namespace botf::app {

AppRendering::AppRendering()
#if SFML_VERSION_MAJOR >= 3
    : m_window{std::make_shared<sf::RenderWindow>(
          sf::VideoMode({static_cast<unsigned int>(common::GlobalSettings::sizeWindowX),
                         static_cast<unsigned int>(common::GlobalSettings::sizeWindowY)}),
          "Battle Of The Flatland")} {
#else
    : m_window{std::make_shared<sf::RenderWindow>(
          sf::VideoMode(static_cast<unsigned int>(common::GlobalSettings::sizeWindowX),
                        static_cast<unsigned int>(common::GlobalSettings::sizeWindowY)),
          "Battle Of The Flatland")} {
#endif
  m_window->setFramerateLimit(60);
}

Player& AppRendering::player() {
  return m_player;
}

const Player& AppRendering::player() const {
  return m_player;
}

InputHandler& AppRendering::input_handler() {
  return m_input_handler;
}

const InputHandler& AppRendering::input_handler() const {
  return m_input_handler;
}

void AppRendering::clear_scene() {
  m_scene.clear();
}

void AppRendering::set_scene_object(common::ID id, common::Point position, common::Angle angle) {
  for (auto& obj : m_scene) {
    if (obj.id == id) {
      obj.position = position;
      obj.angle = angle;
      return;
    }
  }
  m_scene.push_back(SceneObject{id, position, angle});
}

void AppRendering::set_scene(std::vector<SceneObject> objects) {
  m_scene = std::move(objects);
}

bool AppRendering::is_window_open() const {
  return m_window && m_window->isOpen();
}

void AppRendering::draw() {
  if (!m_window) {
    return;
  }

#if SFML_VERSION_MAJOR >= 3
  while (const std::optional event = m_window->pollEvent()) {
    if (event->is<sf::Event::Closed>()) {
      m_window->close();
    }
  }
#else
  sf::Event event{};
  while (m_window->pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      m_window->close();
    }
  }
#endif

  m_window->clear(sf::Color(25, 25, 25));
  draw_map();
  draw_scene_objects();
  m_window->display();

  // Fresh scene each frame; caller fills from WorldState before draw().
  m_scene.clear();
}

void AppRendering::draw_map() {
  // Map rendering later.
}

void AppRendering::draw_scene_objects() {
  for (const auto& el : m_scene) {
    sf::CircleShape circle(10.f);
    circle.setFillColor(sf::Color(120, 20, 20));
#if SFML_VERSION_MAJOR >= 3
    circle.setOrigin({10.f, 10.f});
    circle.setPosition({el.position.x, el.position.y});
#else
    circle.setOrigin(10.f, 10.f);
    circle.setPosition(el.position.x, el.position.y);
#endif
    m_window->draw(circle);
  }
}

}  // namespace botf::app
