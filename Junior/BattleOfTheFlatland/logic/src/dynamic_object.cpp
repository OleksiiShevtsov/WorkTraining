#include "logic/dynamic_object.h"

bof::DynamicObject::DynamicObject( common::ID id, common::Point dot ) :
    m_id{ id },
    m_dot{ dot },
    m_alpha{ 0.0f },
    m_movementState{ common::MovementState::FREE },
    m_movementStatePrev{ common::MovementState::FREE } {}

bof::common::ID bof::DynamicObject::getId() const 
{
    return m_id;
}

bof::common::Point bof::DynamicObject::getDot() const 
{
    return m_dot;
}

bof::common::Angle bof::DynamicObject::getAngle() const 
{
    return m_alpha;
}

bof::common::MovementState bof::DynamicObject::getMovementState() const
{
    return m_movementState;
}

void bof::DynamicObject::setDot(common::Point dot)
{
    m_dot = dot;
}

void bof::DynamicObject::setAngle(common::Angle alpha)
{
    m_alpha = alpha;
}

void bof::DynamicObject::setMovementState(common::MovementState movementState)
{
    m_movementState = movementState;
}


