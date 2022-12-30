#include "vertex_buffer.hpp"

jengine::JBufferElement::JBufferElement(const JShaderDataType _type) :
    type{ _type },
    componentType{ JBufferLayout::shaderDataTypeToComponentsType( _type ) },
    componentCount{ JBufferLayout::shaderDataTypeToComponentsCount( _type ) },
    size{ JBufferLayout::shaderDataTypeSize( _type ) },
    offset{ 0 }
{
}

jengine::JBufferLayout::JBufferLayout(std::initializer_list<JBufferElement> elements) :
    mElements{ std::move(elements) }
{
    size_t offset{ 0 };
    for (auto& element : mElements)
    {
        element.offset = offset;
        offset = offset + element.size;
        mStride = mStride + element.size;
    }
}

constexpr unsigned int jengine::JBufferLayout::shaderDataTypeToComponentsCount(const JShaderDataType type)
{
    switch (type)
    {
    case JShaderDataType::Float:
    case JShaderDataType::Int:
        return 1;
    case JShaderDataType::Float2:
    case JShaderDataType::Int2:
        return 2;
    case JShaderDataType::Float3:
    case JShaderDataType::Int3:
        return 3;
    case JShaderDataType::Float4:
    case JShaderDataType::Int4:
        return 4;
    }

    JLOG_ERROR("shaderDataTypeToComponentsCount: unknown ShaderDataType!");
    return 0;
}

constexpr size_t jengine::JBufferLayout::shaderDataTypeSize(const JShaderDataType type)
{
    switch (type)
    {
    case JShaderDataType::Float:
    case JShaderDataType::Float2:
    case JShaderDataType::Float3:
    case JShaderDataType::Float4:
        return sizeof(GLfloat) * shaderDataTypeToComponentsCount(type);
    case JShaderDataType::Int:
    case JShaderDataType::Int2:
    case JShaderDataType::Int3:
    case JShaderDataType::Int4:
        return sizeof(GLint) * shaderDataTypeToComponentsCount(type);
    }

    JLOG_ERROR("shaderDataTypeSize: unknown ShaderDataType!");
    return 0;
}

constexpr unsigned int jengine::JBufferLayout::shaderDataTypeToComponentsType(const JShaderDataType type)
{
    switch (type)
    {
    case JShaderDataType::Float:
    case JShaderDataType::Float2:
    case JShaderDataType::Float3:
    case JShaderDataType::Float4:
        return GL_FLOAT;
    case JShaderDataType::Int:
    case JShaderDataType::Int2:
    case JShaderDataType::Int3:
    case JShaderDataType::Int4:
        return GL_INT;
    }

    JLOG_ERROR("shaderDataTypeToComponentsType: unknown ShaderDataType!");
    return 0;
}

jengine::JVertexBuffer::JVertexBuffer(const void* data, const size_t size, JBufferLayout bufferLayout, const JEUsage usage):
    mBufferLayout{ std::move( bufferLayout ) }
{
	glGenBuffers(1, &mId);
	glBindBuffer(GL_ARRAY_BUFFER, mId);
	glBufferData(GL_ARRAY_BUFFER, size, data, usageToGLenum(usage));
}

jengine::JVertexBuffer::JVertexBuffer(JVertexBuffer&& vertexBuffer) noexcept :
    mBufferLayout{ vertexBuffer.mBufferLayout },
    mId{ vertexBuffer.mId }
{
	vertexBuffer.mId = 0;
}

jengine::JVertexBuffer& jengine::JVertexBuffer::operator=(JVertexBuffer&& vertexBuffer) noexcept
{
    mBufferLayout = vertexBuffer.mBufferLayout;
	mId = vertexBuffer.mId;
	vertexBuffer.mId = 0;
	return *this;
}

jengine::JVertexBuffer::~JVertexBuffer()
{
	glDeleteBuffers(1, &mId);
}

void jengine::JVertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, mId);
}

void jengine::JVertexBuffer::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLenum jengine::JVertexBuffer::usageToGLenum(const JVertexBuffer::JEUsage usage)
{
	switch (usage)
	{
		case JVertexBuffer::JEUsage::Static: return GL_STATIC_DRAW;
		case JVertexBuffer::JEUsage::Dynamic: return GL_DYNAMIC_DRAW;
		case JVertexBuffer::JEUsage::Stream: return GL_STREAM_DRAW;
	}

	JLOG_ERROR("Unknown JVertexBuffer usage");
	return GL_STREAM_DRAW;
}
