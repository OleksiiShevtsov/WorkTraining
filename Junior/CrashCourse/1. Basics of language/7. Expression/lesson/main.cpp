#include <cstdio>
#include <iostream>
#include <typeinfo>
#include <stdexcept>

#include "checked_integer.h"
#include "heap_alloc.h"
#include "heap_point.h"
#include "cast.h"
#include "constexpr.h"

//volatile prohibits optimizing the code
void f00( volatile int& x ){
    x = 10;// candidate for optimization
    x = 20;
}

int main()
{
    //checkInt();
    //heapAllocCheck();
    //heapPointCheck();
    //checkCast();
    checkConstexpr();

    return 0;
}
