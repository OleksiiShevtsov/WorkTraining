cmake_minimum_required(VERSION 3.16.3)
project(lesson)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES main.cpp
                 cast_function.h
                 simple_unique_pointer.h
                 tracer.h
                 concept.h
                 concept_2.h
)

add_executable(lesson ${SOURCE_FILES})
