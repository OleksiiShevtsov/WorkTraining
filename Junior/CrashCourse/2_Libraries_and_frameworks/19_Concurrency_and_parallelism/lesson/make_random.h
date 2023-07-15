#pragma once

#include <algorithm>
#include <vector>
#include <numeric>
#include <random>
#include <chrono>
#include <iostream>
#include <execution>

#include "stopwatch.h"

std::vector< long > makeRandomVector()
{
    std::vector< long > numbers( 1'000'000'000 );
    std::iota( numbers.begin(), numbers.end(), 0 );
    std::mt19937_64 urng{ 121216 };
    std::shuffle( numbers.begin(), numbers.end(), urng );
    return numbers;
}

void makeRandomCheck()
{
    std::cout << "Constructing random vectors...";
    auto numbers_a = makeRandomVector();
    auto numbers_b{ numbers_a };
    std::chrono::nanoseconds timeToSort;

    std::cout << " " << numbers_a.size() << " elements.\n";

    std::cout << "Sorting with execution::seq...";
    {
        JStopwatch stopwatch{ timeToSort };
        sort( std::execution::seq, numbers_a.begin(), numbers_a.end() );
    }
    std::cout << " took " << timeToSort.count() / 1.0E9 << " sec.\n";

    std::cout << "Sorting with execution::par...";
    {
        JStopwatch stopwatch{ timeToSort };
        sort(std::execution::par, numbers_b.begin(), numbers_b.end());
    }
    std::cout << " took " << timeToSort.count() / 1.0E9 << " sec.\n";
}

// unsynchronized access
void notMagic()
{
    std::vector< long > numbers(1'000'000), squares(1'000'000);
    std::iota(numbers.begin(), numbers.end(), 0);
    std::size_t n_trasformed{};
    std::transform(std::execution::par, numbers.begin(), numbers.end(),
        squares.begin(), [&n_trasformed](const auto n)
        {
            ++n_trasformed;
            return n * n;
        });
    std::cout << "n_trasformed: " << n_trasformed << std::endl;
}