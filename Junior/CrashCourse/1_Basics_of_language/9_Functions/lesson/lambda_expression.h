#pragma once

#include <cstdio>
#include <cstdint>
#include <cstdarg>

template < typename Fn >
void transform( Fn fn, const int* in, int* out, size_t length ){
    for( size_t i{}; i < length; i++ ){
        out[ i ] = fn( in[ i ] );
    }

    for( size_t j{}; j < length; j++ ){
        printf( "%d, ", out[ j ] );
    }
    printf( "\n" );
}

struct LambdaFactory {
    LambdaFactory( char in ) : 
        m_toCount{ in }, m_tally{}{}
    auto makeLamda() {
        // [ = ] - passing by value, 
        // [ & ] - passing by reference
        // [ mutable ] allows to change the 
        // variable captured by value
        return [ this ]( const char* str ) {
            size_t index{}, result{};
            while (str[index]) {
                if ( str[index] == m_toCount) { result++; }
                index++;
            }
            m_tally = m_tally + result;
            return result;
        };
    }
    const char m_toCount;
    rsize_t m_tally;
};

void lambdaExpressionCheck(){

    /*const size_t len{ 3 };
    int base[]{ 1, 2, 3 }, a[ len ], b[ len ], c[ len ];

    auto funcTrans = []( auto x ){ return 10 * x + 5; };

    transform( []( int x ) { return 1; }, base, a, len );
    transform( []( int x ) { return x; }, base, b, len );
    transform( funcTrans, base, c, len );*/

    LambdaFactory factory{ 't' };
    auto lamda = factory.makeLamda();

    printf( "Tally1: %zu\n", factory.m_tally);
    printf( "Sally1: %zu\n", lamda("text1"));

    printf( "Tally2: %zu\n", factory.m_tally);
    printf("Sally2: %zu\n", lamda("text2"));

    printf("Tally: %zu\n", factory.m_tally);
}
