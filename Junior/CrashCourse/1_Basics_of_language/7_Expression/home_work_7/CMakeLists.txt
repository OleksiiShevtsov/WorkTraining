cmake_minimum_required(VERSION 3.16.3)
project(home_work_7)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES main.cpp
                 unsigned_big_integer.h
                 heap_alloc.h
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
