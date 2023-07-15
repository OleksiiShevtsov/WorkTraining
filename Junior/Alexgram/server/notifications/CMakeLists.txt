project(notifications)

file(GLOB notifications_SRC CONFIGURE_DEPENDS
    "include/*"
    "src/*"
)

add_library(${PROJECT_NAME} STATIC ${notifications_SRC})

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME} PUBLIC common)

#add_subdirectory(${CMAKE_SOURCE_DIR}/../common/protobuf ${CMAKE_BINARY_DIR}/notifications/protobuf/output/)
