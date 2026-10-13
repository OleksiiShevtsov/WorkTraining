cmake_minimum_required(VERSION 3.16.3)
project(home_work_4)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp
                 timer.h
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

