project(core LANGUAGES CXX)

file(GLOB core_SRC CONFIGURE_DEPENDS
    "include/*"
    "src/*"
)

add_library(${PROJECT_NAME} STATIC ${core_SRC})

target_include_directories(${PROJECT_NAME} PUBLIC include)
