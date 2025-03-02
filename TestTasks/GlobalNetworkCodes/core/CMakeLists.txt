project(core)

file(GLOB core_SRC CONFIGURE_DEPENDS
    "include/*"
    "src/*"
)

add_library(${PROJECT_NAME} STATIC ${core_SRC})

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME} PUBLIC sqlite3)
