#pragma once

#include <exception>

constexpr void assertThat( bool statement, const char* message ){
    if ( !statement ){
        throw std::runtime_error{ message };
    }
}

void runUserTest( void( *unitUserTest )(), const char* name ){
    try {
        unitUserTest();
        printf( "\x1b[32m[+] Test \"%s\" successful.\x1b[0m\n", name );
    }
    catch( const std::exception& e ) {
        printf( "\x1b[31m[-] Test failure in \"%s\". %s.\x1b[0m\n", name, e.what() );
    }
}

