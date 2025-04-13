cmake_minimum_required(VERSION 3.16)
project(ArrayTransformer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(lib/googletest)

add_subdirectory(core)
add_subdirectory(utils)
add_subdirectory(tests)

set(SOURCE_FILES main.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE core utils)