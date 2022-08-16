#pragma once

namespace flower::common
{
    enum class DirectionSnake {
        LEFT = 0,
        RIGHT,
        UP,
        DOWN,
        MOTIONLESS
    };

    enum class SnakeState{
        READY,
        STAND
    };

    enum class SnakeGrowth{
        READY,
        STAND
    };

    enum class NewSnake{
        READY,
        STAND
    };

    enum class SnakeSpeed{
        NORMAL,
        FORCE
    };

    enum class ButtonState{
        PRESSED,
        FREE
    };

    enum class GameState{
        START,
        PROCESS,
        END
    };
}
