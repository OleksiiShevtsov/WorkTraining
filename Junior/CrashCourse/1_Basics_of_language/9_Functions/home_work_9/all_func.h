#pragma once

#include <cstdio>
#include <iostream>
#include <typeinfo>
#include <stdexcept>

template < typename Fn, typename In >
constexpr bool all( Fn function, In* input, size_t length ){
    for ( size_t i{}; i < length; i++ ) {
        if ( !function( input[ i ] ) ) {
            return false;
        }
    }
    return true;
}

void allCheck(){
    int data[]{ 200, 300, 400, 500 };
    size_t dataLen = 4;
    auto all_gt100 = all( []( auto x ) { return x > 100; }, data, dataLen );
    if (all_gt100){ printf( "All el greater then 100." ); }
    else{ printf( "All el greater non then 100." ); }
}
