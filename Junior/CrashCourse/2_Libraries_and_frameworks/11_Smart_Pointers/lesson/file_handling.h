#pragma once

#include <cstdio>
#include <memory>

using FileGuard = std::unique_ptr< FILE, int( * )( FILE* ) >;

void seyHello( FileGuard file )
{
    fprintf( file.get(), "text" );
}

int fileHandling()
{
    auto file = fopen("text", "w");
    if(!file){ return errno; }

    FileGuard fileGuard{ file, fclose };

    // open file
    seyHello( std::move( fileGuard ) );

    // file close
    return 0;
}
