#pragma once

#include <cstdio>
#include <iostream>
#include <typeinfo>
#include <stdexcept>

struct Color{
    float H, S, V;
};

//constexpr is performed during compilation
constexpr uint8_t max( uint8_t a, uint8_t b ){
    return a > b ? a : b;
}

constexpr uint8_t max( uint8_t a, uint8_t b, uint8_t c ){
    return max( max( a, b ), max( a, c ) );
}

constexpr uint8_t min( uint8_t a, uint8_t b ){
    return a < b ? a : b;
}

constexpr uint8_t min( uint8_t a, uint8_t b, uint8_t c ){
    return min( min( a, b ), min( a, c ) );
}

constexpr float modulo( float dividend, float divisor ){
    const auto quotient = dividend / divisor;
    uint8_t o = static_cast< uint8_t >( quotient );
    return divisor * ( quotient - o );
}

constexpr Color RGBtoHSV( uint8_t r, uint8_t g, uint8_t b ){
    Color c{};
    const auto cMax = max( r, g, b );
    c.V = cMax / 255.0f;
    const auto cMin = min( r, g, b );
    const auto delta = c.V - cMin / 255.0f;
    c.S = cMax == 0 ? 0 : delta / c.V;

    if( cMax == cMin ){
        c.H = 0;
        return c;
    }
    if( cMax == r ){
        c.H = ( g / 255.0f - b / 255.0f ) / delta;
    }else if( cMax == g ){
        c.H = ( b / 255.0f - r / 255.0f ) / delta + 2.0f;
    }else if( cMax == b ){
        c.H = ( r / 255.0f - g / 255.0f ) / delta + 4.0f;
    }
    c.H *= 60.0f;
    c.H = c.H >= 0.0f ? c.H : c.H + 360.0f;
    c.H = modulo( c.H, 360.0f );

    return c;
}

void checkConstexpr(){

    auto black = RGBtoHSV( 0, 0, 0 );
    auto white = RGBtoHSV( 255, 255, 255 );
    auto red = RGBtoHSV( 255, 0, 0 );
    auto green = RGBtoHSV( 0, 255, 0 );
    auto blue = RGBtoHSV( 0, 0, 255 );

    std::cout << "black: H " << black.H << ", S " << black.S << ", V " << black.V << std::endl;
    std::cout << "white: H " << white.H << ", S " << white.S << ", V " << white.V << std::endl;
    std::cout << "red: H " << red.H << ", S " << red.S << ", V " << red.V << std::endl;
    std::cout << "green: H " << green.H << ", S " << green.S << ", V " << green.V << std::endl;
    std::cout << "blue: H " << blue.H << ", S " << blue.S << ", V " << blue.V << std::endl;
}
