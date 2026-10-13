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
    : m_id{id}, m_position{dot}, m_angle{0.0f}, m_speed_limit{speed_limit} {
  m_movement.initial_x = m_position.x;
  m_movement.initial_y = m_position.y;
}

common::ID DynamicObject::id() const {
  return m_id;
}

common::Point DynamicObject::position() const {
  return m_position;
}

common::Angle DynamicObject::angle() const {
  return m_angle;
}

int DynamicObject::input_dx() const {
  return m_input_dx;
}

int DynamicObject::input_dy() const {
  return m_input_dy;
}

void DynamicObject::set_position(common::Point dot) {
  m_position = dot;
}

void DynamicObject::set_angle(common::Angle alpha) {
  m_angle = alpha;
}

void DynamicObject::set_input(int dx, int dy) {
  m_input_dx = clamp_axis(dx);
  m_input_dy = clamp_axis(dy);
}

float DynamicObject::speed_recalculation(float acceleration, float initial_speed) const {
  float speed = acceleration * m_movement.current_time + initial_speed;
  if (m_speed_limit > 0.0f) {
    speed = std::clamp(speed, -m_speed_limit, m_speed_limit);
  }
  return speed;
}

common::Coordinate DynamicObject::coordinate_recalculation(
    float acceleration, float initial_speed, common::Coordinate initial_coordinate) const {
  return acceleration * m_movement.current_time * m_movement.current_time / 2.0f +
         initial_speed * m_movement.current_time + initial_coordinate;
}

void DynamicObject::reset_endpoints() {
  const float speed_mag =
      std::sqrt(m_movement.speed_x * m_movement.speed_x + m_movement.speed_y * m_movement.speed_y);
  if (speed_mag > kSpeedEpsilon) {
    m_movement.cos_acceleration = m_movement.speed_x / speed_mag;
    m_movement.sin_acceleration = m_movement.speed_y / speed_mag;
  } else {
    // No velocity yet — friction direction follows current input if any.
    if (m_input_dx != 0 || m_input_dy != 0) {
      const float input_mag =
          std::sqrt(static_cast<float>(m_input_dx * m_input_dx + m_input_dy * m_input_dy));
      m_movement.cos_acceleration = static_cast<float>(m_input_dx) / input_mag;
      m_movement.sin_acceleration = static_cast<float>(-m_input_dy) / input_mag;  // +dy = up => -y
    } else {
      m_movement.cos_acceleration = 0.0f;
      m_movement.sin_acceleration = 0.0f;
    }
  }

  m_movement.initial_speed_x = m_movement.speed_x;
  m_movement.initial_speed_y = m_movement.speed_y;
  m_movement.initial_x = m_position.x;
  m_movement.initial_y = m_position.y;
  m_movement.current_time = 0.0f;
}

void DynamicObject::update(float dt) {
  if (dt <= 0.0f) {
    return;
  }

  if (m_input_dx != m_prev_input_dx || m_input_dy != m_prev_input_dy) {
    reset_endpoints();
    m_prev_input_dx = m_input_dx;
    m_prev_input_dy = m_input_dy;
  }

  m_movement.current_time += dt;

  const bool idle = (m_input_dx == 0 && m_input_dy == 0);

  if (idle) {
    // FREE: only friction on axes that still move.
    if (std::fabs(m_movement.speed_x) > kSpeedEpsilon) {
      m_movement.speed_x =
          speed_recalculation(kFriction * m_movement.cos_acceleration, m_movement.initial_speed_x);
      m_position.x = coordinate_recalculation(kFriction * m_movement.cos_acceleration,
                                              m_movement.initial_speed_x, m_movement.initial_x);
    }
    if (std::fabs(m_movement.speed_y) > kSpeedEpsilon) {
      m_movement.speed_y =
          speed_recalculation(kFriction * m_movement.sin_acceleration, m_movement.initial_speed_y);
      m_position.y = coordinate_recalculation(kFriction * m_movement.sin_acceleration,
                                              m_movement.initial_speed_y, m_movement.initial_y);
    }
    return;
  }

  // Active input: accelerate on pressed axes (dy=+1 is up → negative screen/game y).
  // Perpendicular residual speed gets friction (old LEFT/RIGHT/UP/DOWN side-effect).
  if (m_input_dx != 0) {
    const float ax = static_cast<float>(m_input_dx) * kAcceleration;
    m_movement.speed_x = speed_recalculation(ax, m_movement.initial_speed_x);
    m_position.x = coordinate_recalculation(ax, m_movement.initial_speed_x, m_movement.initial_x);
  } else if (std::fabs(m_movement.speed_x) > kSpeedEpsilon) {
    // Fixed UP-branch bug: friction on X uses speed_x threshold (not speed_y).
    m_movement.speed_x =
        speed_recalculation(kFriction * m_movement.cos_acceleration, m_movement.initial_speed_x);
    m_position.x = coordinate_recalculation(kFriction * m_movement.cos_acceleration,
                                            m_movement.initial_speed_x, m_movement.initial_x);
  }

  if (m_input_dy != 0) {
    const float ay = static_cast<float>(-m_input_dy) * kAcceleration;
    m_movement.speed_y = speed_recalculation(ay, m_movement.initial_speed_y);
    m_position.y = coordinate_recalculation(ay, m_movement.initial_speed_y, m_movement.initial_y);
  } else if (std::fabs(m_movement.speed_y) > kSpeedEpsilon) {
    m_movement.speed_y =
        speed_recalculation(kFriction * m_movement.sin_acceleration, m_movement.initial_speed_y);
    m_position.y = coordinate_recalculation(kFriction * m_movement.sin_acceleration,
                                            m_movement.initial_speed_y, m_movement.initial_y);
  }
}

}  // namespace botf
