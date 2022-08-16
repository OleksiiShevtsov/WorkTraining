cmake_minimum_required(VERSION 3.16)
project(lesson)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})


add_subdirectory(../../../../../../LIB/googletest ${CMAKE_CURRENT_BINARY_DIR}/googletest)
target_link_libraries(${PROJECT_NAME} PUBLIC gtest gtest_main)

set(Boost_INCLUDE_DIRS ../../../../../../LIB/boost/boost_1_80_0)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
