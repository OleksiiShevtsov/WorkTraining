#include <cstdio>
#include <iostream>
#include <typeinfo>
#include <stdexcept>

#include "test/test.h"
#include "factorize.h"
#include "goat_rodeo.h"
#include "make_random.h"

int main( int argc, char** argv )
{
     goatRodeoCheck();
    //makeRandomCheck();
    // factorizeCheck();
    
	Catch::Session().run(argc, argv);

    return 0;
}
