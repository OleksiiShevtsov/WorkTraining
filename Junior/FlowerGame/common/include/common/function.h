#pragma once

#include <chrono>

namespace flower::common
{
    inline void waiting( const std::chrono::milliseconds waitingTime ){
        auto start = std::chrono::steady_clock::now();
        while ( true )
        {
            auto end = std::chrono::steady_clock::now();
            if ( std::chrono::duration_cast< std::chrono::milliseconds > ( end - start ) > waitingTime )
            {
                break;
            }
        }
    }
}
