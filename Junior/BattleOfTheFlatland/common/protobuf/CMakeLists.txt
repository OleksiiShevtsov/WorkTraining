project(proto_messages)

# Protobuf codegen for length-prefixed game packets over SFML TCP.

set(BOTF_PROTO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/messages)

if(NOT BOTF_LIB_ROOT)
    include("${CMAKE_SOURCE_DIR}/cmake/BotfLibRoot.cmake")
endif()

if(WIN32 AND EXISTS "${BOTF_LIB_ROOT}/protobuf/CMakeLists.txt")
    add_subdirectory(
        "${BOTF_LIB_ROOT}/protobuf"
        ${CMAKE_CURRENT_BINARY_DIR}/protobuf_lib
    )
endif()

find_package(Protobuf REQUIRED)

file(GLOB BOTF_PROTO_FILES CONFIGURE_DEPENDS "${BOTF_PROTO_DIR}/*.proto")

protobuf_generate_cpp(BOTF_PROTO_SRCS BOTF_PROTO_HDRS ${BOTF_PROTO_FILES})

add_library(${PROJECT_NAME} STATIC ${BOTF_PROTO_SRCS} ${BOTF_PROTO_HDRS})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}
)

if(TARGET protobuf::libprotobuf)
    target_link_libraries(${PROJECT_NAME} PUBLIC protobuf::libprotobuf)
else()
    target_link_libraries(${PROJECT_NAME} PUBLIC ${Protobuf_LIBRARIES})
    target_include_directories(${PROJECT_NAME} PUBLIC ${Protobuf_INCLUDE_DIRS})
endif()

message(STATUS "[botf] protobuf: ${Protobuf_VERSION}; protos: ${BOTF_PROTO_FILES}")
