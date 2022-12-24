#include <cstdio>
#include <iostream>
#include <typeinfo>
#include <stdexcept>

#include "test/test.h"

int main( int argc, char** argv )
{
	Catch::Session().run(argc, argv);

    return 0;
}
