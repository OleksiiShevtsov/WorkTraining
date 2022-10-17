cmake_minimum_required(VERSION 3.16.3)
project(jengineCore)

set(CMAKE_CXX_STANDARD 20)

file(GLOB jengineCore_SRC CONFIGURE_DEPENDS
    "include/*"
    "src/*"
)

add_library(${PROJECT_NAME} STATIC ${jengineCore_SRC})

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_directories(${PROJECT_NAME} PUBLIC jengineCore)
