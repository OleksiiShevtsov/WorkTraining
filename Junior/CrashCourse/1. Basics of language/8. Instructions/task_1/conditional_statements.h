#pragma once

#include <iostream>
#include <cstdio>
#include <stdexcept>
#include <type_traits>

template < typename T >
auto valueOf( T x ){
    if constexpr( std::is_pointer<T>::value ){
        if( !x ){ throw std::runtime_error{ "Null pointer dereference." }; }
        return *x;
    } else {
        return x;
    }
}

void conditionalStatementsCheck(){

    unsigned long level{ 8998 };
    auto level_ptr = &level;
    auto &level_ref = level;
    printf( "Power level: %lu\n", valueOf( level_ptr ) );
    ++*level_ptr;
    printf( "Power level: %lu\n", valueOf( level_ref ) );
    ++level_ref;
    printf( "Power level: %lu\n", valueOf( level++ ) );
    try {
        level_ptr = nullptr;
        valueOf( level_ptr );
    }  catch( const std::exception& e ) {
        printf( "Exception: %s\n", e.what() );
    }
}
