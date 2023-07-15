#pragma once

#include <chrono>

#include "common/types.h"

namespace bof::common::GlobalSettings
{
    constexpr size_t sizeWindowX{ 1080 };
    constexpr size_t sizeWindowY{ 640 };
    constexpr std::chrono::milliseconds pauseBetweenButtonPresses{ 50 };
}
