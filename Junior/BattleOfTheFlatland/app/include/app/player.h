#pragma once

#include "common/types.h"

namespace bof::app
{
    class Player
    {
    public:
        Player( const Player& ) = delete;
        Player( Player&& ) = delete;

        Player();

        common::ID getId();

    private:

        common::ID m_id;
    };
}
