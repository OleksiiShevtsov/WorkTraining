#pragma once

#include <cstdio>
#include <memory>

using FileGuard = std::unique_ptr< FILE, int( * )( FILE* ) >;

void fprintToFile( FileGuard file )
{
    fprintf( file.get(), "text" );
}

int fileHandling()
{
    auto file = fopen("text", "w");
    if(!file){ return errno; }

    FileGuard fileGuard{ file, fclose };

    // open file
    fprintToFile( std::move( fileGuard ) );

    // file close
    return 0;
}
