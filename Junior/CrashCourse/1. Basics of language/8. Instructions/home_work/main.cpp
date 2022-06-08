#include <cstdio>
#include <cstddef>
#include <iostream>
#include <cwchar>

#include "range.h"
#include "fibonacci_iterator.h"
#include "prime_number.h"

int main()
{
    FibonacciIterator fibTter{};
    FibonacciIterator* p_fibIter = &fibTter;
    for( const auto element : Range{ p_fibIter, 150 } ){
        printf( "%d | ", element );
    }

    std::cout << std::endl;

    PrimeNumber primeNumberTter{};
    PrimeNumber* p_primeNumberTter = &primeNumberTter;
    for( const auto element : Range{ p_primeNumberTter, 15 } ){
        printf( "%d | ", element );
    }

    return 0;
}
