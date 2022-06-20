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

void lambdaExpressionCheck(){

    /*const size_t len{ 3 };
    int base[]{ 1, 2, 3 }, a[ len ], b[ len ], c[ len ];

    auto funcTrans = []( auto x ){ return 10 * x + 5; };

    transform( []( int x ) { return 1; }, base, a, len );
    transform( []( int x ) { return x; }, base, b, len );
    transform( funcTrans, base, c, len );*/

    char toCount{ 't' };
    size_t tally{};
    // [ = ] - passing by value, [ & ] - passing by reference
    // [ mutable ] allows to change the variable captured by value
    auto sCountr = [ = ]( const char* str ) mutable {
        size_t index{}, result{};
        while ( str[ index ] ){
            if( str[ index ] == toCount ){ result++; }
             index++;
        }
        tally = tally + result;
        return result;
    };

    printf( "Tally: %zu\n", tally );
    auto text1 = sCountr( "text1" );
    printf( "text1: %zu\n", text1 );
    printf( "Tally: %zu\n", tally );
    auto text2 = sCountr( "text2" );
    printf( "text2: %zu\n", text2 );
    printf( "Tally: %zu\n", tally );
}
