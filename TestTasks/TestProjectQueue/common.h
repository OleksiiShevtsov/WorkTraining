#pragma once

#include <cstdint>
#include <time.h>

namespace type {
    using Priority = unsigned char;
    using ID = uint32_t;
    using Ticks = clock_t;
    typedef struct tagTDATA {
        ID id;
        Priority priority;
        Ticks ticks;
        const char* data;
    };
}
