#pragma once

#include <iostream>
#include <cstdio>

void instructionsGotoCheck(){

    goto yoda;

    luke:
        printf( "luke \n" );
    yoda:
        printf( "yoda \n" );
}
