#pragma once

// TODO make a download from a file
namespace jengine::resources
{
    GLfloat positions_colors[] =
    {
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, 0.0f,   0.0f, 1.0f, 0.0f,
    };

    GLuint indices[] =
    {
        0, 1, 2, 3, 2, 1
    };

    const char* vertexShader =
        "#version 460\n"
        "layout(location = 0) in vec3 vertex_position;"
        "layout(location = 1) in vec3 vertex_color;"
        "out vec3 color;"
        "void main() {"
        "   color = vertex_color;"
        "   gl_Position = vec4(vertex_position, 1.0);"
        "}";

    const char* fragmentShader =
        "#version 460\n"
        "in vec3 color;"
        "out vec4 frag_color;"
        "void main() {"
        "   frag_color = vec4(color, 1.0);"
        "}";
}