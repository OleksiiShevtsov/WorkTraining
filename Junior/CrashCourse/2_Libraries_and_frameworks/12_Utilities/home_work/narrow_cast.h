#pragma once

#include <optional>

template< typename To, typename From >
std::optional< To > narrowCast( From value ) 
{
    const auto converted = static_cast< To >( value );
    const auto backwards = static_cast< From >( converted );
    if ( value != backwards ) 
    {
        return std::nullopt;
    }
    printf( "%s -> %s -> %s\n", 
        typeid( value ).name(), 
        typeid( converted ).name(), 
        typeid( backwards ).name() );
    return converted;
}
