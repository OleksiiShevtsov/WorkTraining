#include <cstdio>
#include <iostream>
#include <typeinfo>
#include <stdexcept>

#include "test/test.h"
#include "stopwatch.h"

void performanceAdvance()
{
    size_t size{ 100'000 };

    std::vector< int > vec( size, 1 );
    std::forward_list< int > list( size, 1 );

    std::chrono::nanoseconds elapsedVec;
    std::chrono::nanoseconds elapsedList;

    {
        JStopwatch stopwatch{ elapsedVec };
        for( size_t i{ 1 }; i < size; i++ )
        {
            auto itr = vec.begin();
            std::advance( itr, i );
        }
    }

    int resVec = std::chrono::duration_cast< std::chrono::milliseconds >( elapsedVec ).count();
    printf( "%dms per advance for vector.\n", resVec );

    {
        JStopwatch stopwatch{ elapsedList };
        for( size_t i{ 1 }; i < size; i++ )
        {
            auto itr = list.begin();
            std::advance( itr, i );
        }
    }

    int resList = std::chrono::duration_cast< std::chrono::milliseconds >( elapsedList ).count();
    printf( "%dms per advance for list.\n", resList );
}

int main( int argc, char** argv )
{
    performanceAdvance();

    Catch::Session().run( argc, argv );

    return 0;
}

