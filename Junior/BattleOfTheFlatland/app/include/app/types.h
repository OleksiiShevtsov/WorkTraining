#pragma once

#include <vector>

#include "common/types.h"

namespace bof::app 
{
    using DynamicObject = struct 
    {
        common::ID id; common::Point dot; common::Angle alpha;
    };
}
