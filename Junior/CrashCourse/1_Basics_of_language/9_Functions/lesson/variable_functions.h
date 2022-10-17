#include <cstdio>
#include <cstdint>
#include <cstdarg>

// variable function
/*int sum( size_t n, ... ){
    va_list args;

    va_start( args, n );
    int result{ 0 };
    while ( n-- ){
        auto next_element = va_arg( args, int );
        result = result + next_element;
    }

    va_end( args );

    return result;
}*/

// variable template
/*template < typename T >
constexpr T sum( T x ){ return x; }

template < typename T, typename... Args >
constexpr T sum( T x, Args... args ){
    return x + sum( args ... );
}*/

//expression of package
template < typename... T >
constexpr auto sum( T... args ){
    return ( ... + args );
}

void variableFunctionsCheck(){
    printf( "sum: %d\n", sum( 3, 4, 5 ) );
}
