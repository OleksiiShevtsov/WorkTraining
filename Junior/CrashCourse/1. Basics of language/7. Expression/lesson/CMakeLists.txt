cmake_minimum_required(VERSION 3.16.3)
project(lesson)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp
                 checked_integer.h
                 heap_alloc.h
                 heap_point.h
                 cast.h
                 constexpr.h
)

add_executable(lesson ${SOURCE_FILES})
