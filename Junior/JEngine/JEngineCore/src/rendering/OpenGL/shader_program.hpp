#pragma once

#include "jengineCore/log.h"
#include "jengineCore/types.h"

#include <glad/glad.h>

namespace jengine
{
    class JShaderProgram
    {
    public:
        JShaderProgram(const char* vertexShaderSrc, const char* fragmentShaderSrc);
        JShaderProgram(JShaderProgram&&) noexcept;
        JShaderProgram& operator=(JShaderProgram&&) noexcept;
        ~JShaderProgram();

        JShaderProgram() = delete;
        JShaderProgram(const JShaderProgram&) = delete;
        JShaderProgram& operator=(const JShaderProgram&) = delete;

        void bind() const;
        static void unbind();
        bool isCompiled() const { return mIsCompiled; }

    private:

        static bool createShader(const char* source, const GLenum shaderType, GLuint& shaderId);

        bool mIsCompiled{ false };
        types::ID mId{ 0 };
    };
}