#include <cstdio>
#include <iostream>
#include <typeinfo>
#include <stdexcept>

#include "stopwatch.h"
#include "test/test_utility.h"
#include "test/test_data.h"

int main( int argc, char** argv )
{
	std::chrono::nanoseconds elapsed;

	{
		JStopwatch stopwatch{ elapsed };
		Catch::Session().run(argc, argv);
	}
	
	int res = std::chrono::duration_cast< std::chrono::milliseconds >( elapsed ).count();
	printf( "Took %dms per division.", res );

    return 0;
}
