#pragma once

#include <cstdio>
#include <limits>
#include <iostream>
#include <stdexcept>

struct CheckedInteger{
    CheckedInteger( unsigned int value ) : m_value{ value } {}

    CheckedInteger operator+( unsigned int other ){
        CheckedInteger result{ m_value + other };
        if( result.m_value < m_value ){ throw std::overflow_error{ "overflow:" }; }
        return result;
    }
    unsigned int get() const { return m_value; }

private:
    const unsigned int m_value;
};

void checkInt()
{
    CheckedInteger a{ 100 };
    auto b = a + 200;
    printf( "a + 200 = %u\n", b.get() );

    try {
        auto c = a + std::numeric_limits<unsigned int>::max();
    }catch (std::exception& e) {
        printf( "(a + max) exception: %s\n", e.what());
    }
}
