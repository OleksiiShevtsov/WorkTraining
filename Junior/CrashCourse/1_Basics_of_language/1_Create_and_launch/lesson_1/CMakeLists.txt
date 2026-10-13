cmake_minimum_required(VERSION 3.16.3)
project(lesson_1)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

