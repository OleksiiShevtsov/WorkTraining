cmake_minimum_required(VERSION 3.16.3)
project(task_1)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES main.cpp
                 decltype.h
                 variable_functions.h
                 pointer_to_function.h
                 function_call_operator.h
                 lambda_expression.h
)

add_executable(task_1 ${SOURCE_FILES})
