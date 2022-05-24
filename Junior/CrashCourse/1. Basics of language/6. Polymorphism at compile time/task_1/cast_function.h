#pragma once

#include <cwchar>
#include <cstdio>
#include <typeinfo>
#include <stdexcept>

// const_cast
void constCastFunctional ( int& num ){
    printf("const_cast:         type name: %s\n", typeid( num ).name());
    auto &constNum = const_cast< const int& >( num );
    printf("const_cast:         type name: %s\n", typeid( constNum ).name());
}

// static_cast
void staticCastFunctional ( void* target ){
    printf("static_cast:        type name: %s\n", typeid( target ).name());
    auto targetSort = static_cast< short* >( target );
    printf("static_cast:        type name: %s\n", typeid( targetSort ).name());
}

// reinterpret_cast
void reinterpretCastFunctional ( int* num ){
    printf("reinterpret_cast:   type name: %s\n", typeid( num ).name());
    auto charNum = reinterpret_cast< char* >( &num );
    printf("reinterpret_cast:   type name: %s\n", typeid( charNum ).name());
}

// narrow_cast
template< typename To, typename From >
To narrow_cast ( From value ){
    const auto converted = static_cast< To >( value );
    const auto backwards = static_cast< From >( converted );
    if( value != backwards ){
        throw std::runtime_error{ "Narrowed!" };
    }
    printf("%s -> %s -> %s\n", typeid( value ).name(), typeid( converted ).name(), typeid( backwards ).name());
    return converted;
}

void checkNarrowCast(){
    int perfect{ 496 };
    const auto perfectShort = narrow_cast< short >( perfect );
    printf("perfect short: %d\n", perfectShort);
    try {
        int cyclic{ 142857 };
        const auto cyclicShort = narrow_cast< short >( cyclic );
        printf("cyclic short: %d\n", cyclicShort);
    }
    catch ( const std::runtime_error& e ) {
        printf( "Exception: %s\n", e.what() );
    }
}
