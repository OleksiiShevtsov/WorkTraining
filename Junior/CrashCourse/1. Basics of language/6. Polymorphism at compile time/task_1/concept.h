#pragma once

#include <type_traits>
#include <cstdio>
#include <cstdint>
#include <cstddef>

//template< typename T1, typename T2>
//concept bool conceptName(){
//    return false;
//}

constexpr const char* asStr( bool value ){
    return value ? "true" : "false";
}

void checkType(){
    printf( "is_integral< int > : %s\n", asStr( std::is_integral< int >::value ) );
    printf( "is_integral< int* >: %s\n", asStr( std::is_integral< int* >::value ) );
}

// concept for mean
/*template < typename T >
concept bool averageable(){
    return std::is_default_constructible< T >::value
        && requires ( T a, T b ){
            { a += b } -> T;
            { a / size_t{ 1 } } -> T;
    };
}*/

// mean
/*template < averageable T >
T mean( const T* values, size_t length ){
    T result{};
    for(size_t i{}; i < length; i++ ){
        result += values[i];
    }
    return result / length;
}*/
