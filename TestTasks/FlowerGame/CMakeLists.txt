cmake_minimum_required(VERSION 3.16)
project(flower_game)

set(CMAKE_CXX_STANDARD 20)

set(LIB_ROOT "$ENV{CMAKE_LIB_ROOT}")
if(NOT LIB_ROOT)
    message(FATAL_ERROR "Переменная CMAKE_LIB_ROOT не задана")
endif()
file(TO_CMAKE_PATH "${LIB_ROOT}" LIB_ROOT)
message(STATUS "LIB_ROOT = ${LIB_ROOT}")

add_subdirectory("${LIB_ROOT}/googletest" googletest-build)
if(WIN32)
    add_subdirectory("${LIB_ROOT}/SFML" sfml-build)
endif()

add_subdirectory(logic)
add_subdirectory(app)
add_subdirectory(common)
add_subdirectory(tests)

set(SOURCE_FILES main.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE logic app common)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/app/fonts"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/app/fonts"
    COMMENT "Copying fonts to output directory"
)