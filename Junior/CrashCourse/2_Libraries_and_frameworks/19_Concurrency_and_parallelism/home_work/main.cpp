#include <cstdio>
#include <iostream>
#include <typeinfo>
#include <stdexcept>

#include "spin_lock.h"
#include "safe queue.h"

int main( int argc, char** argv )
{
    safeQueueCheck();
    // spinLockCheck();

    return 0;
}
