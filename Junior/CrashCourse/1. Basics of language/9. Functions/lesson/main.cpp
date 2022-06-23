#include <cstdio>
#include <iostream>
#include <typeinfo>
#include <stdexcept>

#include "decltype.h"
#include "variable_functions.h"
#include "pointer_to_function.h"
#include "function_call_operator.h"
#include "lambda_expression.h"
#include "std_function.h"
#include "alpha_histogram.h"

// The function is compiled in a series of instructions
// 1. Move arguments to registers and calls stack
// 2. Insert the return address to the calls stack
// 3. Go to the defiant function
// 4. After the function is completed, go to the return address
// 5. Clean the calls stack

// * for optimization
// inline       - function call is replaced by its body, this happens at compile time
// constexpr    - the function must be executed at compile time
// noexcept     - the function never throws an exception
// const        - the method will not modify an instance of its class
// volatile     - method operates on mutable objects
// final        - method that cannot be overridden ( same for classes )
// override     - the function must override the virtual function
// virtual      - function can be overridden in child class ( the most child class is called )

//decltypeCheck();
//variableFunctionsCheck();
//pointerToFunctionCheck();
//functionCallOperatorCheck();
//lambdaExpressionCheck();
//stdFunctionCheck();

// program parameters
int main( int argc, char** argv )
{
    AlphaHistogram hist;

    for( size_t i {}; i < argc; i++ ){
        hist.insert( argv[ i ] );
    }
    hist.print();

    return 0;
}
