project(common)

file(GLOB COMMON_INCLUDES CONFIGURE_DEPENDS
    "include/*"
)

add_library(${PROJECT_NAME} INTERFACE ${COMMON_INCLUDES})

target_include_directories(${PROJECT_NAME} INTERFACE include)

