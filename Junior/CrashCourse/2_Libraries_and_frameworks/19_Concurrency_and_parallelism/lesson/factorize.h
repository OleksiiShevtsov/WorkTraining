#pragma once

#include <set>
#include <array>
#include <vector>
#include <iostream>
#include <limits>
#include <chrono>
#include <sstream>
#include <string>

#include "stopwatch.h"

template < typename T >
std::set< T > factorize( T n )
{
    std::set< T > result{ 1 };
    for( T candidate{ 2 }; candidate <= n; candidate++ )
    {
        if( n % candidate == 0 )
        {
            result.insert( candidate );
            n /= candidate;
            candidate = 1;
        }
    }
    return result;
}

std::string factorTask( unsigned long n )
{
    std::chrono::nanoseconds elapsedNs;
    std::set< unsigned long > factors;
    {
        JStopwatch stopwatch{ elapsedNs };
        factors = factorize( n );
    }
    const auto elapsedMs = std::chrono::duration_cast< std::chrono::milliseconds >( elapsedNs ).count();
    std::stringstream ss;
    ss << elapsedMs << " ms: Factoring " << n << " ( ";
    for( auto factor : factors ) ss << factor << " ";
    ss << ")\n";
    return ss.str();
}

std::array< unsigned long, 4 > numbers
{
    9'699'690,
    179'426'549,
    1'000'000'007,
    4'294'967'291
};

void factorizeCheck()
{
    std::cout << "start factor task...\n";
    std::chrono::nanoseconds elapsedNs;
    {
        JStopwatch stopwatch{ elapsedNs };
        std::vector< std::future< std::string > > factorTasks;
        for( auto number : numbers )
        {
            factorTasks.emplace_back( std::async( std::launch::async, factorTask, number ) );
        }
        for ( auto& task : factorTasks )
        {
            std::cout << task.get();
        }
    }
    const auto elapsedMs = std::chrono::duration_cast< std::chrono::milliseconds >( elapsedNs ).count();
    std::cout << elapsedMs << " ms: total program time\n";
}
