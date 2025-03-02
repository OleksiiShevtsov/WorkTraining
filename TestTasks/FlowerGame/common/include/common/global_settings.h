#pragma once

#include <chrono>

#include "common/types.h"

namespace flower::common::GlobalSettings
{
    constexpr size_t pixelCountX{ 32 };
    constexpr size_t pixelCountY{ 24 };
    constexpr size_t sizePixel{ 30 };
    constexpr double coefficientOfReductionAppearanceNewSnake{ 1 + 0.05 };
    constexpr std::chrono::milliseconds timeToDecreaseTimeOfAppearanceNewSnake{ 10000 };
    constexpr std::chrono::milliseconds timeForNewCoins{ 3000 };
    constexpr std::chrono::milliseconds timeForSpeedForce{ 5000 };
    constexpr std::chrono::milliseconds timeForSnakeGrowth{ 3000 };
    constexpr std::chrono::milliseconds timeForNewSnake{ 5000 };
    constexpr std::chrono::milliseconds snakeSpeedNormal{ 200 };
    constexpr std::chrono::milliseconds snakeSpeedForce{ snakeSpeedNormal / 2 };
    constexpr std::chrono::milliseconds pauseBetweenButtonPresses{ 50 };

    namespace Coins
    {
        constexpr ID coinsForOneFlower{ 5 };
        constexpr ID initialSupplyOfCoins{ 150 };
        constexpr ID flowerCost{ 50 };
    }
}
