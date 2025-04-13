project(utils LANGUAGES CXX)

file(GLOB utils_SRC CONFIGURE_DEPENDS
    "include/*"
    "src/*"
)

add_library(${PROJECT_NAME} STATIC ${utils_SRC})

target_include_directories(${PROJECT_NAME} PUBLIC include)