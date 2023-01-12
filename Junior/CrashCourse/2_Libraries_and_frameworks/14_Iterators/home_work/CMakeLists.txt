cmake_minimum_required(VERSION 3.16.3)
project(home_work)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES main.cpp
)

add_executable(home_work ${SOURCE_FILES})

add_subdirectory(../../../../../../LIB/Catch2 ${CMAKE_CURRENT_BINARY_DIR}/Catch2)
target_link_libraries(${PROJECT_NAME} Catch2::Catch2)
