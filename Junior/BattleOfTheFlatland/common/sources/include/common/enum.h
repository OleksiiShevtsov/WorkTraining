#pragma once

namespace bof::common
{
    enum class MovementState
    {
        UP = 0,
        DOWN,
        LEFT,
        RIGHT,
        FREE
    };

    enum class GameState
    {
        START,
        PROCESS,
        END
    };
}
