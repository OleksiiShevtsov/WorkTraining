#pragma once

#include <cstdio>
#include <iostream>
#include <typeinfo>
#include <stdexcept>
#include <vector>

struct HistogramOfArgument{
private:
    struct Argument;
public:
    void insert( int argc, char** argv ){
        for( size_t i{}; i < argc; i++ ){
            m_argument.push_back( Argument{ argv[ i ], strlen( argv[ i ] ) } );
        }
    }
    void print() const {
        for( const auto& el : m_argument ){
            printf( "%s, %d;\n", el.m_argv, el.m_size );
        }
    }
private:
    struct Argument{
        char* m_argv;
        size_t m_size;
    };
    std::vector< Argument > m_argument;
};
