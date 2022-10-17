#include <cstdio>
#include <cstdint>
#include <cstdarg>

float add( float f, int i ){
    return f + i;
}

float subtract( float f, int i ){
    return f - i;
}

void pointerToFunctionCheck(){
    const float first{ 100 };
    const int second{ 20 };

    float( *operation )( float, int ){};
    //using operation = float( * )( float, int );
    printf( "operation initialized to: 0x%p\n", operation );
    operation = &add;
    printf( "&add 0x%p\n", operation );
    printf( "%g + %d = %g\n", first, second, operation( first, second ) );

    operation = subtract;
    printf( "&subtract 0x%p\n", operation );
    printf( "%g - %d = %g\n", first, second, operation( first, second ) );
}
