#pragma once

#include <iostream>

struct RandomNumberGenerator{
    explicit RandomNumberGenerator( uint32_t seed )
        : m_iterations{}, m_number{ seed }{}
    uint32_t next();
    size_t getIterations() const;
private:
    size_t m_iterations;
    uint32_t m_number;
};

uint32_t RandomNumberGenerator::next() {
    ++m_iterations;
    m_number = 0x3FFFFFFF & ( 0x41C64E6D * m_number + 12345 ) % 0x80000000;
    return m_number;
}

size_t RandomNumberGenerator::getIterations() const {
    return m_iterations;
}

void randomNumberCheck(){

    RandomNumberGenerator rng( 100 );
    while ( rng.next() != rng.getIterations() ) {}
    printf( "%zu \n", rng.getIterations() );
}
