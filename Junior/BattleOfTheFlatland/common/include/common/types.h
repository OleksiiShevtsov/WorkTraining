#pragma once

#include <cstdint>

namespace bof::common {
    using Coordinate = uint32_t;
    using ID = uint32_t;
    using Angle = float;
    typedef struct { Coordinate x; Coordinate y; } Point;
}
