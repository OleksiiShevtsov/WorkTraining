cmake_minimum_required(VERSION 3.8)
project(client)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} "async_client.h" "sync_client.h")

set(Boost_INCLUDE_DIRS ../../../../LIB/boost/boost_1_82_0)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})