cmake_minimum_required(VERSION 3.16.3)
project(task_1)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp
                 decltype.h
                 variable_functions.h
)

add_executable(task_1 ${SOURCE_FILES})
