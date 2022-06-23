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

    ColorStorage observeShrub( const char* name ){
        return ColorStorage{ name, Color::BLUE };
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

    const char* description;

    switch ( const auto newColor = rgbColor::observeShrub( "MY_BLUE" ); newColor.m_color ) {
        case SC::RED   : { description = "NEW_RED";   }; break;
        case SC::GREEN : { description = "NEW_GREEN"; }; break;
        case SC::BLUE  : { description = "NEW_BLUE";  }; break;
        default: { description = "ERROR_COLOR"; };
    }

    printf( "COLOR: %s\n", description );
}
