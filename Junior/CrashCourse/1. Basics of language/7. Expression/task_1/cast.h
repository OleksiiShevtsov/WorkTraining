#pragma once

#include <cstdio>
#include <iostream>
#include <typeinfo>
#include <stdexcept>
#include <limits>

struct ReadOnlyInt{
    ReadOnlyInt( int value ) : m_value{ value } {}
    explicit operator int() const { return m_value; }
private:
    const int m_value;
};

void checkCast(){
    ReadOnlyInt myInt{ 2 };
    printf( "myIntNew: %d\n", static_cast< int >( myInt ) * 10 );
}
