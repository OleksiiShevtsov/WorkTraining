project(proto_messages)

add_subdirectory(../../../../../LIB/protobuf  ${CMAKE_CURRENT_BINARY_DIR}/protobuf)
find_package(protobuf REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

message(STATUS "Using Protocol Buffers ${protobuf_VERSION}")

set(protobuf_output_dir ${CMAKE_BINARY_DIR}/notifications/protobuf/output/)
file(MAKE_DIRECTORY ${protobuf_output_dir})

set(protobuf_templates_dir ${CMAKE_SOURCE_DIR}/notifications/protobuf/messages/)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${protobuf_templates_dir})

file(GLOB messages_proto CONFIGURE_DEPENDS "messages/*.proto")
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${messages_proto})

list(LENGTH PROTO_SRCS PROTO_SRCS_LEN)
message(STATUS "[Protobuf] ${PROTO_SRCS_LEN} cpp files were generated")

add_library(${PROJECT_NAME} STATIC ${PROTO_SRCS} ${PROTO_HDRS})

# Выводим сгенерированные пути
message(STATUS "Generated Protobuf file: ${PROTO_SRCS}")

# Включение директории с заголовками и линковка с библиотекой Protobuf
target_include_directories(${PROJECT_NAME} PUBLIC output ${protobuf_output_dir})
target_link_libraries(${PROJECT_NAME} PUBLIC protobuf)
