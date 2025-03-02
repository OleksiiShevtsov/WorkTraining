project(app)

file(GLOB app_SRC CONFIGURE_DEPENDS
    "include/*"
    "src/*"
)

add_library(${PROJECT_NAME} STATIC ${app_SRC})

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PUBLIC Qt::Widgets core)
