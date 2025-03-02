cmake_minimum_required(VERSION 3.16)
project(flower_game)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib/googletest)

if(WIN32)
    add_subdirectory(lib/SFML/)
endif()

add_subdirectory(logic)
add_subdirectory(app)
add_subdirectory(common)
add_subdirectory(tests)

set(SOURCE_FILES main.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE logic app common)
