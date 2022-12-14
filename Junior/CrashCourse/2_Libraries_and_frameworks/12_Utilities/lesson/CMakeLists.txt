cmake_minimum_required(VERSION 3.16.3)
project(lesson)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES main.cpp
)

add_executable(lesson ${SOURCE_FILES})

add_subdirectory(../../../../../../LIB/catch/Catch2 ${CMAKE_CURRENT_BINARY_DIR}/Catch2)
target_link_libraries(${PROJECT_NAME} Catch2::Catch2)

set(Boost_INCLUDE_DIRS ../../../../../../LIB/boost/boost_1_80_0)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})