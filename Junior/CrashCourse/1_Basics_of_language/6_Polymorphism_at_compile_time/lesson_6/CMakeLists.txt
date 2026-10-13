cmake_minimum_required(VERSION 4.0)
project(lesson_6)

set(CMAKE_CXX_STANDARD 23)

set(SOURCE_FILES main.cpp
                 cast_function.h
                 simple_unique_pointer.h
                 tracer.h
                 concept.h
                 concept_2.h
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
