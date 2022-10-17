#pragma once

#include <cstdio>
#include <iostream>
#include <typeinfo>
#include <stdexcept>

template < typename Fn, typename In, typename Out >
constexpr Out fold( Fn function, In* input, size_t length, Out initial ){

    Out resSum{}, min = input[ 0 ], max = input[ 0 ], numOfElMoreThan200{};

    for ( size_t i {}; i < length; i++ ){
        resSum = function( resSum, input[ i ] );
        if( input[ i ] > 200 ){ numOfElMoreThan200++; }
        if( i == 0 ){ continue; }
        if( min > input[ i ] ){ min = input[ i ]; }
        if( max < input[ i ] ){ max = input[ i ]; }
    }
    printf( "min: %d\n", min );
    printf( "max: %d\n", max );
    printf( "number of elements is more than 200: %d\n", numOfElMoreThan200 );

    return resSum;
}

void foldCheck(){
    int data[]{ 100, 200, 300, 400, 500 };
    size_t dataLen = 5;
    auto sum = fold( []( auto x, auto y ){ return x + y; }, data, dataLen, 0 );
    printf( "sum: %d\n", sum );
}
