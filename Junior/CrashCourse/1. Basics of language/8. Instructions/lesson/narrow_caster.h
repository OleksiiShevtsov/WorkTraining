#pragma once

#include <cstdio>
#include <iostream>
#include <stdexcept>

template < typename To, typename From >
struct NarrowCaster {
    To cast( From value ) {
        const auto converted = static_cast< To >( value );
        const auto backwards = static_cast< From >( converted );
        if( value != backwards ){ throw std::runtime_error{ "Narrowed!" }; };
        return converted;
    }
};

template < typename From >
using short_caster1 = NarrowCaster< short, From >;

void narrowCasterCheck(){

    try{
        NarrowCaster< short, int > c;
        c.cast(142857);

        const short_caster1< int > caster;
        //const auto a = caster.cast(1); not work
    }
    catch ( const std::runtime_error& e ) {
        printf( "Exception: %s\n", e.what() );
    }
}
