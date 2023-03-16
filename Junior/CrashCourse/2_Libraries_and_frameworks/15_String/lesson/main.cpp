#include <cstdio>
#include <iostream>
#include <typeinfo>
#include <stdexcept>

#include "test/test_string.h"

void checkReturnDataAndCstr()
{
    std::string word( "pulchritudinous" );
    printf( "c_str: %s at 0x%p\n", word.c_str(), word.c_str() );
    printf( "data: %s at 0x%p\n", word.data(), word.data() );
}

int main( int argc, char** argv )
{
    checkReturnDataAndCstr();

    Catch::Session().run(argc, argv);

    return 0;
}
