#pragma once

#include <cstdio>
#include <memory>

using FileGuard = std::shared_ptr< FILE >;

struct Hal
{
    Hal(std::shared_ptr<FILE> file) : 
        mFile{ file }{}

    ~Hal()
    {
        fprintf( mFile.get(), "I.m completely operational.\n" );
    }

    void writeStatus()
    {
        fprintf( mFile.get(), "Stop Alex\n" );
    }

    std::shared_ptr<FILE> mFile;
};

void fprintToFile( FileGuard file )
{
    Hal hal( file );
    hal.writeStatus();
}

int fileHandling()
{
    auto file = fopen("file.txt", "w");
    if(!file)
    {
        return errno; 
    }

    FileGuard fileGuard{ file, fclose };

    fprintToFile( fileGuard );
    fprintToFile( fileGuard );

    return 0;
}
