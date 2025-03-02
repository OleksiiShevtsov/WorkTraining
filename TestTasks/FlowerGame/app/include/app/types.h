#pragma once

#include <vector>

#include "common/types.h"

namespace flower::app {
    using ObjSnake = struct{ common::ID id; std::vector< common::Point > items; };
    using ObjFlower = struct{ common::ID id; common::Point item; };
}
