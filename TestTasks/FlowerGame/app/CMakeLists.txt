project(app)

file(GLOB app_SRC CONFIGURE_DEPENDS
    "include/*.h"
    "src/*.cpp"
)

add_library(${PROJECT_NAME} STATIC ${app_SRC})

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME} PUBLIC sfml-graphics sfml-window sfml-system PRIVATE common)
