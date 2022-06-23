#include <cstdio>
#include <cstddef>
#include <iostream>
#include <typeinfo>
#include <cwchar> // for wprintf

#include "tracer.h"
#include "cast_function.h"
#include "simple_unique_pointer.h"
#include "concept.h"
#include "concept_2.h"

template<typename T>
T square( T value ){
    return value * value;
}

//template < typename T >
//requires is_copy_constructibl< T >::value

int main()
{
    /*int nonConstNum = 100;
    constCastFunctional( nonConstNum );

    short shortValue{ 100 };
    staticCastFunctional( &shortValue );

    int numReinterpret = 100;
    reinterpretCastFunctional( &numReinterpret );

    checkNarrowCast();*/

    //const int arrInt[]{ 10, 20, 30 };
    //printf( "mean: %d\n", mean( arrInt, sizeof( arrInt )/sizeof( int ) ) );

    //SimpleUniquePointer< Tracer > tracerPointer{ new Tracer("text") };
    //printf( "tracer: 0x%p\n", tracerPointer.getPointer() );

    //char myChar{ 'q' };
    //std::cout << square( myChar );

    //checkType();
    checkConcept2();

    return 0;
}
