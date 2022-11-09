#pragma once

#include "jengineCore/log.h"
#include "jengineCore/types.h"

#include <glad/glad.h>

namespace jengine
{
    enum class JShaderDataType
    {
        Float,
        Float2,
        Float3,
        Float4,
        Int,
        Int2,
        Int3,
        Int4,
    };

    struct JBufferElement
    {
        JBufferElement(const JShaderDataType type);
        JShaderDataType type;
        uint32_t componentType;
        size_t componentCount;
        size_t size;
        size_t offset;
    };

    class JBufferLayout
    {
    public:
        JBufferLayout(std::initializer_list<JBufferElement> elements);

        const std::vector<JBufferElement>& getElement() const { return mElements; }
        size_t getStribe() const { return mStride; }

        static constexpr unsigned int shaderDataTypeToComponentsCount(const JShaderDataType type);
        static constexpr size_t shaderDataTypeSize(const JShaderDataType type);
        static constexpr unsigned int shaderDataTypeToComponentsType(const JShaderDataType type);

    private:
        std::vector<JBufferElement> mElements;
        size_t mStride{ 0 };
    };

    class JVertexBuffer
    {
    public:

        enum class JEUsage
        {
            Static,
            Dynamic,
            Stream
        };

        JVertexBuffer(const void* data, const size_t size, JBufferLayout bufferLayout, const JEUsage usage = JVertexBuffer::JEUsage::Static);
        JVertexBuffer(JVertexBuffer&&) noexcept;
        JVertexBuffer& operator=(JVertexBuffer&&) noexcept;
        ~JVertexBuffer();

        JVertexBuffer(const JVertexBuffer&) = delete;
        JVertexBuffer& operator=(const JVertexBuffer&) = delete;

        void bind() const;
        static void unbind();

        const JBufferLayout& getLayout() const { return mBufferLayout; }

    private:

        static GLenum usageToGLenum(const JVertexBuffer::JEUsage usage);

        types::ID mId{ 0 };
        JBufferLayout mBufferLayout;
    };
}