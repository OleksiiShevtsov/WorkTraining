#pragma once

#include "vertex_buffer.hpp"

namespace jengine
{
    class JIndexBuffer
    {
    public:

        JIndexBuffer(const void* data, const size_t count, const JVertexBuffer::JEUsage usage = JVertexBuffer::JEUsage::Static);
        JIndexBuffer(JIndexBuffer&&) noexcept;
        JIndexBuffer& operator=(JIndexBuffer&&) noexcept;
        ~JIndexBuffer();

        JIndexBuffer(const JIndexBuffer&) = delete;
        JIndexBuffer& operator=(const JIndexBuffer&) = delete;

        void bind() const;
        static void unbind();

        size_t getCount() const { return mCount; }

    private:

        constexpr static GLenum usageToGLenum(const JVertexBuffer::JEUsage usage);

        types::ID mId{ 0 };
        size_t mCount;
    };
}