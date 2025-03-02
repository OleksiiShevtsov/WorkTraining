project(logic)

file(GLOB logic_SRC CONFIGURE_DEPENDS
    "include/*"
    "src/*"
)

add_library(${PROJECT_NAME} STATIC ${logic_SRC})

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME} PUBLIC common PRIVATE app)
