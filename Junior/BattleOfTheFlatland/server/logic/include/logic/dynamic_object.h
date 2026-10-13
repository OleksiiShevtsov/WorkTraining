#pragma once

#include "common/types.hpp"

namespace botf {

// Movable entity with acceleration / inertia (from old DynamicObject, adapted to dx/dy + dt).
class DynamicObject {
 public:
  DynamicObject(common::ID id, common::Point dot, float speed_limit);

  [[nodiscard]] common::ID id() const;
  [[nodiscard]] common::Point position() const;
  [[nodiscard]] common::Angle angle() const;
  [[nodiscard]] int input_dx() const;
  [[nodiscard]] int input_dy() const;

  void set_position(common::Point dot);
  void set_angle(common::Angle alpha);

  // Movement intent for this tick: each axis in {-1, 0, 1}.
  void set_input(int dx, int dy);

  // Advance simulation by fixed dt (seconds).
  void update(float dt);

 private:
  struct MovementParameters {
    float cos_acceleration{};
    float sin_acceleration{};
    float speed_x{};
    float speed_y{};
    float initial_speed_x{};
    float initial_speed_y{};
    common::Coordinate initial_x{};
    common::Coordinate initial_y{};
    float current_time{};
  };

  float speed_recalculation(float acceleration, float initial_speed) const;
  common::Coordinate coordinate_recalculation(float acceleration,
                                              float initial_speed,
                                              common::Coordinate initial_coordinate) const;
  void reset_endpoints();

  static constexpr float kSpeedEpsilon{2.0f};
  static constexpr float kAcceleration{50.0f};
  static constexpr float kFriction{-9.81f / 0.4f};

  common::ID id_;
  common::Point position_{};
  common::Angle angle_{0.0f};
  float speed_limit_;

  int input_dx_{0};
  int input_dy_{0};
  int prev_input_dx_{0};
  int prev_input_dy_{0};

  MovementParameters movement_{};
};

}  // namespace botf
