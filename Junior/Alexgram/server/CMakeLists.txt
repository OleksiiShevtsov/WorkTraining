cmake_minimum_required(VERSION 3.16)
project(server)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(logic)
add_subdirectory(notifications)
add_subdirectory(../common ${CMAKE_CURRENT_BINARY_DIR}/common)

set(SOURCE_FILES main.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    notifications
    logic
)

set(Boost_INCLUDE_DIRS ../../../../LIB/boost/boost_1_80_0)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
