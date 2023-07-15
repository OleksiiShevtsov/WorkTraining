#include <cstdio>
#include <iostream>
#include <typeinfo>
#include <stdexcept>

#include "sync_server.h"
#include "async_server.h"
#include "async_client.h"

int main( int argc, char** argv )
{
    syncServerStart();

    return 0;
}
