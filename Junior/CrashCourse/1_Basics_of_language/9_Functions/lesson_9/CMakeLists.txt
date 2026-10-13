cmake_minimum_required(VERSION 3.16.3)
project(lesson_9)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES main.cpp
                 decltype.h
                 variable_functions.h
                 pointer_to_function.h
                 function_call_operator.h
                 lambda_expression.h
                 std_function.h
                 alpha_histogram.h
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
