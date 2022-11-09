cmake_minimum_required(VERSION 3.16.3)
project(jengineCore)

set(CMAKE_CXX_STANDARD 20)

set(JENGINE_PUBLIC_INCLUDES
    "include/jengineCore/application.h"
    "include/jengineCore/event.h"
    "include/jengineCore/log.h"
    "include/jengineCore/types.h"
    "include/jengineCore/settings.h"
    "include/jengineCore/gui.h"
)

set(JENGINE_PRIVATE_INCLUDES
    "src/window.hpp"
    "src/rendering/OpenGL/index_buffer.hpp"
    "src/rendering/OpenGL/shader_program.hpp"
    "src/rendering/OpenGL/vertex_buffer.hpp"
    "src/rendering/OpenGL/vertex_array.hpp"
    "src/rendering/resources/resources.hpp"
)

set(JENGINE_PRIVATE_SOURCES
    "src/application.cpp"
    "src/window.cpp"
    "src/gui.cpp"
    "src/rendering/OpenGL/index_buffer.cpp"
    "src/rendering/OpenGL/shader_program.cpp"
    "src/rendering/OpenGL/vertex_array.cpp"
    "src/rendering/OpenGL/vertex_buffer.cpp"
)

set(JENGINE_ALL_SOURCES
    ${JENGINE_PUBLIC_INCLUDES}
    ${JENGINE_PRIVATE_INCLUDES}
    ${JENGINE_PRIVATE_SOURCES}
)

add_library(${PROJECT_NAME} STATIC
    ${JENGINE_ALL_SOURCES}
)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PRIVATE src)

add_subdirectory(../external/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)

add_subdirectory(../external/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
target_link_libraries(${PROJECT_NAME} PUBLIC glad)

add_subdirectory(../external/spdlog ${CMAKE_CURRENT_BINARY_DIR}/spdlog)
target_link_libraries(${PROJECT_NAME} PUBLIC spdlog)

add_subdirectory(../external/glm ${CMAKE_CURRENT_BINARY_DIR}/glm)
target_link_libraries(${PROJECT_NAME} PUBLIC glm)

# Creating a static library ImGui
set(IMGUI_INCLUDES
    "../external/imgui/imgui.h"
    "../external/imgui/backends/imgui_impl_glfw.h"
    "../external/imgui/backends/imgui_impl_opengl3.h"
)

set(IMGUI_SOURCES
    "../external/imgui/imgui.cpp"
    "../external/imgui/backends/imgui_impl_glfw.cpp"
    "../external/imgui/backends/imgui_impl_opengl3.cpp"
    "../external/imgui/imgui_demo.cpp"
    "../external/imgui/imgui_draw.cpp"
    "../external/imgui/imgui_tables.cpp"
    "../external/imgui/imgui_widgets.cpp"
)

add_library(imgui STATIC
    ${IMGUI_INCLUDES}
    ${IMGUI_SOURCES}
)

target_include_directories(imgui PUBLIC ../external)
target_include_directories(imgui PUBLIC ../external/imgui)

# include glad end glfw in imgui
target_link_libraries(imgui PRIVATE glad glfw)

# include imgui in jengineCore
target_link_libraries(${PROJECT_NAME} PRIVATE imgui)
