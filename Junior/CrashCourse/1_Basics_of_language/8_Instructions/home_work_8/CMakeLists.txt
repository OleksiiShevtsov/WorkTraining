cmake_minimum_required(VERSION 3.16.3)
project(home_work_8)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp
                 fibonacci_iterator.h
                 prime_number.h
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
