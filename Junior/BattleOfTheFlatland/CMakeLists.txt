cmake_minimum_required(VERSION 3.16)
project(BattleOfTheFlatland)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include("${CMAKE_SOURCE_DIR}/cmake/BotfLibRoot.cmake")

# Guard: binary dir must not be inside the source tree (use sibling BUILD/).
get_filename_component(_botf_src "${CMAKE_SOURCE_DIR}" ABSOLUTE)
get_filename_component(_botf_bin "${CMAKE_BINARY_DIR}" ABSOLUTE)
string(FIND "${_botf_bin}" "${_botf_src}/" _botf_bin_inside)
if(_botf_bin_inside EQUAL 0)
    message(WARNING
        "[botf] Build directory is inside the project:\n"
        "  ${CMAKE_BINARY_DIR}\n"
        "Preferred layout uses a sibling BUILD folder:\n"
        "  cmake -S . -B \"${BOTF_BUILD_ROOT}/BattleOfTheFlatland\"\n"
        "Or open CMake preset 'default' / 'vs-default'.")
endif()

function(botf_add_lib_subdir name)
    set(_candidates
        "${BOTF_LIB_ROOT}/${name}"
        "${BOTF_LIB_ROOT}/${name}"
    )
    if(name STREQUAL "SFML")
        list(APPEND _candidates "${BOTF_LIB_ROOT}/sfml")
        file(GLOB _sfml_ver_dirs "${BOTF_LIB_ROOT}/SFML-*")
        list(APPEND _candidates ${_sfml_ver_dirs})
    endif()

    foreach(_dir IN LISTS _candidates)
        if(EXISTS "${_dir}/CMakeLists.txt")
            message(STATUS "[botf] Using ${name} from ${_dir}")
            add_subdirectory("${_dir}" "${CMAKE_BINARY_DIR}/_deps/${name}")
            return()
        endif()
    endforeach()
    message(WARNING "[botf] ${name} not found under ${BOTF_LIB_ROOT}. Run setup.bat (it skips clone if already present).")
endfunction()

if(WIN32)
    botf_add_lib_subdir(SFML)
    botf_add_lib_subdir(googletest)
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
