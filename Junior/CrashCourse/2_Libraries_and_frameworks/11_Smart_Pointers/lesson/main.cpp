#include <cstdio>
#include <iostream>
#include <typeinfo>
#include <stdexcept>

#include "test/test_dead_men_of_dunharrow.h"

int main(int argc, char *argv[])
{
    Catch::Session().run(argc, argv);
    return 0;
}
