cmake_minimum_required(VERSION 3.5)
project(ptnServer)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ptnNetwork
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        ptnNetwork
)
