#include "logic/dynamic_object.h"

#include <algorithm>
#include <cmath>

namespace botf {
namespace {

int clamp_axis(int value) {
  return std::clamp(value, -1, 1);
}

}  // namespace

DynamicObject::DynamicObject(common::ID id, common::Point dot, float speed_limit)
    : id_{id},
      position_{dot},
      angle_{0.0f},
      speed_limit_{speed_limit} {
  movement_.initial_x = position_.x;
  movement_.initial_y = position_.y;
}

common::ID DynamicObject::id() const {
  return id_;
}

common::Point DynamicObject::position() const {
  return position_;
}

common::Angle DynamicObject::angle() const {
  return angle_;
}

int DynamicObject::input_dx() const {
  return input_dx_;
}

int DynamicObject::input_dy() const {
  return input_dy_;
}

void DynamicObject::set_position(common::Point dot) {
  position_ = dot;
}

void DynamicObject::set_angle(common::Angle alpha) {
  angle_ = alpha;
}

void DynamicObject::set_input(int dx, int dy) {
  input_dx_ = clamp_axis(dx);
  input_dy_ = clamp_axis(dy);
}

float DynamicObject::speed_recalculation(float acceleration, float initial_speed) const {
  float speed = acceleration * movement_.current_time + initial_speed;
  if (speed_limit_ > 0.0f) {
    speed = std::clamp(speed, -speed_limit_, speed_limit_);
  }
  return speed;
}

common::Coordinate DynamicObject::coordinate_recalculation(
    float acceleration,
    float initial_speed,
    common::Coordinate initial_coordinate) const {
  return acceleration * movement_.current_time * movement_.current_time / 2.0f +
         initial_speed * movement_.current_time + initial_coordinate;
}

void DynamicObject::reset_endpoints() {
  const float speed_mag = std::sqrt(movement_.speed_x * movement_.speed_x +
                                    movement_.speed_y * movement_.speed_y);
  if (speed_mag > kSpeedEpsilon) {
    movement_.cos_acceleration = movement_.speed_x / speed_mag;
    movement_.sin_acceleration = movement_.speed_y / speed_mag;
  } else {
    // No velocity yet — friction direction follows current input if any.
    if (input_dx_ != 0 || input_dy_ != 0) {
      const float input_mag =
          std::sqrt(static_cast<float>(input_dx_ * input_dx_ + input_dy_ * input_dy_));
      movement_.cos_acceleration = static_cast<float>(input_dx_) / input_mag;
      movement_.sin_acceleration = static_cast<float>(-input_dy_) / input_mag;  // +dy = up => -y
    } else {
      movement_.cos_acceleration = 0.0f;
      movement_.sin_acceleration = 0.0f;
    }
  }

  movement_.initial_speed_x = movement_.speed_x;
  movement_.initial_speed_y = movement_.speed_y;
  movement_.initial_x = position_.x;
  movement_.initial_y = position_.y;
  movement_.current_time = 0.0f;
}

void DynamicObject::update(float dt) {
  if (dt <= 0.0f) {
    return;
  }

  if (input_dx_ != prev_input_dx_ || input_dy_ != prev_input_dy_) {
    reset_endpoints();
    prev_input_dx_ = input_dx_;
    prev_input_dy_ = input_dy_;
  }

  movement_.current_time += dt;

  const bool idle = (input_dx_ == 0 && input_dy_ == 0);

  if (idle) {
    // FREE: only friction on axes that still move.
    if (std::fabs(movement_.speed_x) > kSpeedEpsilon) {
      movement_.speed_x = speed_recalculation(kFriction * movement_.cos_acceleration,
                                              movement_.initial_speed_x);
      position_.x = coordinate_recalculation(kFriction * movement_.cos_acceleration,
                                             movement_.initial_speed_x,
                                             movement_.initial_x);
    }
    if (std::fabs(movement_.speed_y) > kSpeedEpsilon) {
      movement_.speed_y = speed_recalculation(kFriction * movement_.sin_acceleration,
                                              movement_.initial_speed_y);
      position_.y = coordinate_recalculation(kFriction * movement_.sin_acceleration,
                                             movement_.initial_speed_y,
                                             movement_.initial_y);
    }
    return;
  }

  // Active input: accelerate on pressed axes (dy=+1 is up → negative screen/game y).
  // Perpendicular residual speed gets friction (old LEFT/RIGHT/UP/DOWN side-effect).
  if (input_dx_ != 0) {
    const float ax = static_cast<float>(input_dx_) * kAcceleration;
    movement_.speed_x = speed_recalculation(ax, movement_.initial_speed_x);
    position_.x = coordinate_recalculation(ax, movement_.initial_speed_x, movement_.initial_x);
  } else if (std::fabs(movement_.speed_x) > kSpeedEpsilon) {
    // Fixed UP-branch bug: friction on X uses speed_x threshold (not speed_y).
    movement_.speed_x = speed_recalculation(kFriction * movement_.cos_acceleration,
                                            movement_.initial_speed_x);
    position_.x = coordinate_recalculation(kFriction * movement_.cos_acceleration,
                                           movement_.initial_speed_x,
                                           movement_.initial_x);
  }

  if (input_dy_ != 0) {
    const float ay = static_cast<float>(-input_dy_) * kAcceleration;
    movement_.speed_y = speed_recalculation(ay, movement_.initial_speed_y);
    position_.y = coordinate_recalculation(ay, movement_.initial_speed_y, movement_.initial_y);
  } else if (std::fabs(movement_.speed_y) > kSpeedEpsilon) {
    movement_.speed_y = speed_recalculation(kFriction * movement_.sin_acceleration,
                                            movement_.initial_speed_y);
    position_.y = coordinate_recalculation(kFriction * movement_.sin_acceleration,
                                           movement_.initial_speed_y,
                                           movement_.initial_y);
  }
}

}  // namespace botf
