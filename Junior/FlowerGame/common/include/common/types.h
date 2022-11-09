#pragma once

#include <cstdint>
#include <vector>

namespace flower::common {
    using Coordinate = uint32_t;
    using Coin = uint32_t;
    using ID = uint32_t;
    using Point = struct { Coordinate x; Coordinate y; };
}