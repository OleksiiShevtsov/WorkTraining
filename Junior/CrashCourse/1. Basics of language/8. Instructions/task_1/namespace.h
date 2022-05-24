#pragma once

#include <iostream>

namespace spaceColor::rgbColor {
    enum class Color{
        RED,
        GREEN,
        BLUE
    };

    struct ColorStorage{
        const char* m_name;
        Color m_color;
    };

    bool isColorRed( const ColorStorage& color ){
        return color.m_color == Color::RED;
    }
}

using namespace spaceColor;
using String = const char[ 512 ];
using SC = rgbColor::Color;

void namespaceCheck(){

    const rgbColor::ColorStorage myColor{ "MY_RED", SC::RED };
    String strRed{ "spaceColor::Color::RED:" };

    if( rgbColor::isColorRed( myColor ) ){
        printf( "%s %d \n", strRed, SC::RED );
    }
}
