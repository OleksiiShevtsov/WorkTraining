#include <cstdio>
#include <iostream>
#include <typeinfo>
#include <stdexcept>

#include "test/test_narrow_cast.h"
#include "random_pass.h"

int main( int argc, char** argv )
{
    //Catch::Session().run(argc, argv);

    randomPassCheck();

    return 0;
}
