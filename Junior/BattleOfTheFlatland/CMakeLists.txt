cmake_minimum_required(VERSION 3.16)
project(BattleOfTheFlatland)

set(CMAKE_CXX_STANDARD 20)

if(WIN32)
    add_subdirectory(../../../LIB/SFML  ${CMAKE_CURRENT_BINARY_DIR}/SFML)
    add_subdirectory(../../../LIB/googletest  ${CMAKE_CURRENT_BINARY_DIR}/googletest)
    #add_subdirectory(../../../LIB/protobuf  ${CMAKE_CURRENT_BINARY_DIR}/protobuf)
endif()

add_subdirectory(common/sources ${CMAKE_CURRENT_BINARY_DIR}/common)
add_subdirectory(common/protobuf ${CMAKE_CURRENT_BINARY_DIR}/proto_messages)

add_subdirectory(logic)
add_subdirectory(app)
#add_subdirectory(notifications)
add_subdirectory(tests)

set(SOURCE_FILES main.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE logic app common proto_messages)
