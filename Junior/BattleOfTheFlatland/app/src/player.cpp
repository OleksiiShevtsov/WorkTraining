#pragma once

#include "app/player.h"

bof::app::Player::Player() :
    m_id{ 0 }{}

bof::common::ID bof::app::Player::getId()
{
    return m_id;
}
