cmake_minimum_required(VERSION 3.16.3)
project(TestProjectQueue)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp
                 client.h
                 client.cpp
                 queue.h
                 queue.cpp
                 server.h
                 server.cpp
                 log.txt
)

add_executable(TestProjectQueue ${SOURCE_FILES})

find_package(Threads REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} Threads::Threads)

