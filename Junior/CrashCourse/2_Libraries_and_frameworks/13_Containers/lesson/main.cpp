#include <cstdio>
#include <iostream>
#include <typeinfo>
#include <stdexcept>

#include "test/test_array.h"
#include "test/test_vector.h"

int main( int argc, char** argv )
{
	Catch::Session().run(argc, argv);

    return 0;
}
