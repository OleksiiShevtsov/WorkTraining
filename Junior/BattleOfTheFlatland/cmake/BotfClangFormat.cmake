# Optional clang-format pass before building botf_server / botf_client / tests.
#
#   BOTF_FORMAT_ON_BUILD=ON  (default) — format sources, then compile
#   BOTF_FORMAT_ON_BUILD=OFF — skip (still can run: cmake --build ... --target botf_format)
#
# Requires clang-format on PATH. If missing, configure continues without failing.

option(BOTF_FORMAT_ON_BUILD "Run clang-format before building BOTF targets" ON)

find_program(BOTF_CLANG_FORMAT_EXE
    NAMES clang-format clang-format-18 clang-format-17 clang-format-16
    DOC "clang-format executable for BOTF"
)

set(_botf_format_globs
    "${CMAKE_SOURCE_DIR}/client/*.cpp"
    "${CMAKE_SOURCE_DIR}/client/*.h"
    "${CMAKE_SOURCE_DIR}/client/*.hpp"
    "${CMAKE_SOURCE_DIR}/client/app/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/client/app/include/app/*.h"
    "${CMAKE_SOURCE_DIR}/client/app/include/app/*.hpp"
    "${CMAKE_SOURCE_DIR}/server/*.cpp"
    "${CMAKE_SOURCE_DIR}/server/*.h"
    "${CMAKE_SOURCE_DIR}/server/*.hpp"
    "${CMAKE_SOURCE_DIR}/server/logic/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/server/logic/include/logic/*.h"
    "${CMAKE_SOURCE_DIR}/server/logic/include/logic/*.hpp"
    "${CMAKE_SOURCE_DIR}/net/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/net/include/net/*.h"
    "${CMAKE_SOURCE_DIR}/net/include/net/*.hpp"
    "${CMAKE_SOURCE_DIR}/common/sources/include/common/*.h"
    "${CMAKE_SOURCE_DIR}/common/sources/include/common/*.hpp"
    "${CMAKE_SOURCE_DIR}/tests/*.cpp"
    "${CMAKE_SOURCE_DIR}/tests/*.h"
    "${CMAKE_SOURCE_DIR}/tests/*.hpp"
    "${CMAKE_SOURCE_DIR}/tests/helpers/*.h"
    "${CMAKE_SOURCE_DIR}/tests/helpers/*.hpp"
)

file(GLOB_RECURSE BOTF_FORMAT_SOURCES CONFIGURE_DEPENDS ${_botf_format_globs})
list(FILTER BOTF_FORMAT_SOURCES EXCLUDE REGEX "/build/")
list(REMOVE_DUPLICATES BOTF_FORMAT_SOURCES)

if(NOT BOTF_CLANG_FORMAT_EXE)
    message(STATUS "[botf] clang-format not found — format-on-build disabled (install LLVM / clang-format)")
    return()
endif()

if(NOT BOTF_FORMAT_SOURCES)
    message(STATUS "[botf] clang-format: no sources matched")
    return()
endif()

message(STATUS "[botf] clang-format: ${BOTF_CLANG_FORMAT_EXE}")
message(STATUS "[botf] format-on-build: ${BOTF_FORMAT_ON_BUILD}")

add_custom_target(botf_format
    COMMAND "${BOTF_CLANG_FORMAT_EXE}" -i ${BOTF_FORMAT_SOURCES}
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    COMMENT "[botf] clang-format (${CMAKE_SOURCE_DIR})"
    VERBATIM
)

if(BOTF_FORMAT_ON_BUILD)
    # Every target that compiles our sources must wait for format (no race with -i).
    foreach(_botf_tgt IN ITEMS logic app net botf_server botf_client tests)
        if(TARGET ${_botf_tgt})
            add_dependencies(${_botf_tgt} botf_format)
        endif()
    endforeach()
endif()
