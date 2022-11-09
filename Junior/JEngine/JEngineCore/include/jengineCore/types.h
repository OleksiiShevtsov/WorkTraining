// move to a separate directory
#pragma once

#include "jengineCore/event.h"

#include <string>
#include <cstdint>

namespace jengine::types 
{
    using JEventCallBackFn = std::function< void( JBaseEvent& ) >;
    using JColor = float[4];
    using ID = uint32_t;

    struct JSizeWindow
    {
        unsigned int width;
        unsigned int height;
    };

    struct JWindowData
    {
        JSizeWindow sizeWindow;
        std::string title;
        JEventCallBackFn eventCallbackFn;
    };
}
