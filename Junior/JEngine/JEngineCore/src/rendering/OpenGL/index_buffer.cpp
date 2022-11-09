#include "index_buffer.hpp"

jengine::JIndexBuffer::JIndexBuffer(const void* data, const size_t count, const JVertexBuffer::JEUsage usage):
	mCount{ count }
{
	glGenBuffers(1, &mId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, usageToGLenum(usage));
}

jengine::JIndexBuffer::JIndexBuffer(JIndexBuffer&& indexBuffer) noexcept :
    mId{ indexBuffer.mId },
	mCount{ indexBuffer.mCount }
{
	indexBuffer.mId = 0;
	indexBuffer.mCount = 0;
}

jengine::JIndexBuffer& jengine::JIndexBuffer::operator=(JIndexBuffer&& indexBuffer) noexcept
{
	mId = indexBuffer.mId;
	mCount = indexBuffer.mCount;
	indexBuffer.mId = 0;
	indexBuffer.mCount = 0;
	return *this;
}

jengine::JIndexBuffer::~JIndexBuffer()
{
	glDeleteBuffers(1, &mId);
}

void jengine::JIndexBuffer::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mId);
}

void jengine::JIndexBuffer::unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

constexpr GLenum jengine::JIndexBuffer::usageToGLenum(const JVertexBuffer::JEUsage usage)
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
