#pragma once

#include "jengineCore/log.h"
#include "jengineCore/types.h"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"

#include <glad/glad.h>

namespace jengine
{
    class JVertexArray
    {
    public:
        JVertexArray();
        JVertexArray(JVertexArray&&) noexcept;
        JVertexArray& operator=(JVertexArray&&) noexcept;
        ~JVertexArray();

        JVertexArray(const JVertexArray&) = delete;
        JVertexArray& operator=(const JVertexArray&) = delete;

        void addVertexBuffer(const JVertexBuffer& vertexBuffer);
        void setIndexBuffer(const JIndexBuffer& indexBuffer);
        void bind() const;
        static void unbind();
        size_t getIndicesCount() const { return mIndicesCount; }

    private:
        types::ID mId{ 0 };
        unsigned int mElementsCount{ 0 };
        size_t mIndicesCount{ 0 };
    };
}