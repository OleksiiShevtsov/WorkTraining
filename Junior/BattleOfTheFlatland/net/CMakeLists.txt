project(net)

# Transport: SFML TCP + inbound/outbound notification queues + length-prefixed frames.

if(NOT TARGET sfml-network)
    find_package(SFML 2.5 COMPONENTS network system REQUIRED)
endif()

file(GLOB net_SRC CONFIGURE_DEPENDS
    "src/*.cpp"
)

add_library(${PROJECT_NAME} STATIC ${net_SRC})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        include
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        common
        proto_messages
        sfml-network
        sfml-system
)
