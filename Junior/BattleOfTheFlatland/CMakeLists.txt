cmake_minimum_required(VERSION 3.16)
project(BattleOfTheFlatland)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include("${CMAKE_SOURCE_DIR}/cmake/BotfLibRoot.cmake")

if(WIN32)
    if(EXISTS "${BOTF_LIB_ROOT}/SFML/CMakeLists.txt")
        add_subdirectory("${BOTF_LIB_ROOT}/SFML" ${CMAKE_CURRENT_BINARY_DIR}/SFML)
    else()
        message(WARNING "[botf] SFML not found in ${BOTF_LIB_ROOT}. Run setup.bat")
    endif()
    if(EXISTS "${BOTF_LIB_ROOT}/googletest/CMakeLists.txt")
        add_subdirectory("${BOTF_LIB_ROOT}/googletest" ${CMAKE_CURRENT_BINARY_DIR}/googletest)
    else()
        message(WARNING "[botf] googletest not found in ${BOTF_LIB_ROOT}. Run setup.bat")
    endif()
else()
    find_package(SFML 2.5 COMPONENTS system window graphics network REQUIRED)
    find_package(GTest QUIET)
endif()

# Shared libraries
add_subdirectory(common/sources ${CMAKE_CURRENT_BINARY_DIR}/common)
add_subdirectory(common/protobuf ${CMAKE_CURRENT_BINARY_DIR}/proto_messages)
add_subdirectory(net)
add_subdirectory(tests)

# Executables (client owns app/, server owns logic/)
add_subdirectory(server)
add_subdirectory(client)
