#include <cstdio>
#include <iostream>
#include <typeinfo>
#include <stdexcept>

#include "test/test.h"
#include "make_timer.h"
#include "blocking_hostname_resolution.h"
#include "connection.h"
#include "simple_client.h"
#include "simple_server.h"

int main( int argc, char** argv )
{
    // makeTimerCheck();
    // blockingHostnameResolutionCheck();
    // connectionCheck();
    simplClientCheck();
    // simplServerCheck();
    
	// Catch::Session().run(argc, argv);

    return 0;
}
