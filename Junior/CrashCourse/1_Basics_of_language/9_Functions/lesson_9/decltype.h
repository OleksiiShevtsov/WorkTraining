#pragma once

#include <cstdio>

template < typename X, typename Y >
auto add( X x, Y y ) -> decltype ( x + y ) {
    return x + y;
}

void decltypeCheck(){
    auto num1 = add( 100., -10 );
    printf( "type num1: %d | typname: %s\n", num1, typeid( num1 ).name() );

    auto num2 = add( 100U, -20 );
    printf( "type num2: %d | typname: %s\n", num2, typeid( num2 ).name() );

    auto num3 = add( char{ 100 }, 54'999'900ull );
    printf( "type num3: %d | typname: %s\n", num3, typeid( num3 ).name() );
}
