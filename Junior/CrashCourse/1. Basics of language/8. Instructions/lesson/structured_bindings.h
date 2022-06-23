#pragma once

#include <iostream>

struct TextFile{
    bool seccess;
    const char* data;
    size_t n_bytes;
};

TextFile readTextFile( const char* path ){
    const static char data[]{ "text" };
    return TextFile{
        true,
        data,
        sizeof ( data )
    };
}

void structuredBindingsCheck(){

    // [seccess, txt, leng] exists only in if()
    if( const auto[ seccess, txt, leng ] = readTextFile( "REAMDE.txt" ); seccess ){
        printf( "size: %zu, contents: %s\n", leng, txt );
    } else {
        printf( "Failed to open REAMDE.txt.\n" );
    }

}
