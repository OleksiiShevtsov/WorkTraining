cmake_minimum_required(VERSION 3.8)
project(lesson)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp

)

add_executable(lesson ${SOURCE_FILES} "async_client.h" "async_server.h")

set(Boost_INCLUDE_DIRS ../../../../LIB/boost/boost_1_82_0)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})