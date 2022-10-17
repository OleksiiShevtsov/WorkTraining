cmake_minimum_required(VERSION 3.16)
project(home_work)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

add_subdirectory(../../../../../../LIB/googletest ${CMAKE_CURRENT_BINARY_DIR}/googletest)
target_link_libraries(${PROJECT_NAME} PUBLIC gtest gtest_main gmock)
