cmake_minimum_required(VERSION 3.8)
project(lesson)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp
)

add_executable(lesson ${SOURCE_FILES})

add_subdirectory(../../../../../../LIB/Catch2 ${CMAKE_CURRENT_BINARY_DIR}/Catch2)
target_link_libraries(${PROJECT_NAME} Catch2::Catch2)