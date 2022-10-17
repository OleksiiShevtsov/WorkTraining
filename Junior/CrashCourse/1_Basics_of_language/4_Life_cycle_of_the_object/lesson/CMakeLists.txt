cmake_minimum_required(VERSION 3.16.3)
project(lesson)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp
                 simple_string.h
                 simple_string_owner.h
                 jball.h
)

add_executable(lesson ${SOURCE_FILES})
