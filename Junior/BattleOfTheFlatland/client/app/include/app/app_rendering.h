#pragma once

#include "app/input_handler.h"
#include "app/player.h"
#include "app/types.h"

#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

namespace botf::app {

class AppRendering {
 public:
  AppRendering();

  AppRendering(const AppRendering&) = delete;
  AppRendering(AppRendering&&) = delete;

  [[nodiscard]] Player& player();
  [[nodiscard]] const Player& player() const;
  [[nodiscard]] InputHandler& input_handler();
  [[nodiscard]] const InputHandler& input_handler() const;

  // Scene for this frame (typically filled from WorldState).
  void clear_scene();
  void set_scene_object(common::ID id, common::Point position, common::Angle angle);
  void set_scene(std::vector<SceneObject> objects);

  [[nodiscard]] bool is_window_open() const;
  void draw();

 private:
  void draw_map();
  void draw_scene_objects();

  Player player_{};
  InputHandler input_handler_{};
  std::vector<SceneObject> scene_;
  std::shared_ptr<sf::RenderWindow> window_;
};

}  // namespace botf::app
