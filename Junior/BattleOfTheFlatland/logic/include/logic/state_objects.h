#pragma once

#include "logic/dynamic_object.h"

namespace bof 
{
    class StateObjects{
    public:

        StateObjects( const uint32_t pixelCountX, const uint32_t pixelCountY );

    private:
        uint32_t m_pixelCountY;
        uint32_t m_pixelCountX;
    };
}
