#pragma once

#include <iostream>
#include <array>

bool doubleReturnOverflow( uint8_t& x ){
    const auto original = x;
    x = x * 2;
    return original > x;
}

void cycleCheck(){

    uint8_t x{ 1 };
    printf( "uint8_t: \n===\n" );
    do{
        printf( "%u ", x );
    }while( !doubleReturnOverflow( x ) );
    std::cout << std::endl;

    std::array arr{ 1, 1, 2, 3, 5, 8 };
    /////////////////////////////
    for( const auto element : arr ){
        printf( "%d ", element );
    }
    /////////////////////////////
    std::cout << std::endl;
    /////////////////////////////
    const auto e = arr.end();
    for( auto i = arr.begin(); i != e; ++i ){
        const auto& element = *i;
        printf( "%d ", element );
    }
}
