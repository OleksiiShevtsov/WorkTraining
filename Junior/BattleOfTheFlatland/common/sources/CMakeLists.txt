project(common)

file(GLOB_RECURSE COMMON_INCLUDES CONFIGURE_DEPENDS
    "include/*.h"
    "include/*.hpp"
)

if(COMMON_INCLUDES)
    add_library(${PROJECT_NAME} INTERFACE ${COMMON_INCLUDES})
else()
    add_library(${PROJECT_NAME} INTERFACE)
endif()

target_include_directories(${PROJECT_NAME} INTERFACE include)
