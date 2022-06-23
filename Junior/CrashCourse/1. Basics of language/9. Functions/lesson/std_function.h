#pragma once

#include <cstdio>
#include <cstdint>
#include <functional>

struct CountIf {
    CountIf( char c ) : m_c{ c }{}
    // #1 element in an array, object of function
    size_t operator()( const char* str ) const {
        size_t index{ 0 }, result{ 0 };
        while ( str[ index ] ){
            if( str[ index ] == m_c ) { result++; }
            index++;
        }
        return result;
    }
private:
    const char m_c;
};

// #2 element in an array, just a function
size_t countScaces( const char* str ){
    size_t index{}, result{};
    while( str[ index ] ){
        if( str[ index ] == ' ' ){ result++; }
        index++;
    }
    return result;
}

// three functions array
std::function < size_t( const char* ) > funcs[]{
    countScaces,
    CountIf{ 'e' },
    // #3 element in an array, lambda function
    []( const char* str ){
        size_t index{};
        while ( str[ index ] ){ index++; }
        return index;
    }
};

auto text = "text 1, text 2";

void stdFunctionCheck(){
    size_t index{};

    // Conclusion of functions
    for( const auto& func : funcs ){
        printf( "func #%zu: %zu\n", index++, func( text ) );
    }
}
