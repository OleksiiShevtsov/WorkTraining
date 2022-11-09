#include "vertex_array.hpp"

jengine::JVertexArray::JVertexArray()
{
	glGenVertexArrays(1, &mId);
}

jengine::JVertexArray::JVertexArray(JVertexArray&& vertexArray) noexcept : 
	mId{ vertexArray.mId },
	mElementsCount{ vertexArray.mElementsCount }
{
	vertexArray.mId = 0;
	vertexArray.mElementsCount = 0;
}

jengine::JVertexArray& jengine::JVertexArray::operator=(JVertexArray&& vertexArray) noexcept
{
	mId = vertexArray.mId,
	mElementsCount = vertexArray.mElementsCount;

	vertexArray.mId = 0;
	vertexArray.mElementsCount = 0;
	return *this;
}

jengine::JVertexArray::~JVertexArray()
{
	glDeleteVertexArrays(1, &mId);
}

void jengine::JVertexArray::addVertexBuffer(const JVertexBuffer& vertexBuffer)
{
	bind();
	vertexBuffer.bind();

	for (const JBufferElement& currentElement : vertexBuffer.getLayout().getElement())
	{
		glEnableVertexAttribArray(mElementsCount);
		glVertexAttribPointer(
			mElementsCount,
			static_cast<GLint>(currentElement.componentCount),
			currentElement.componentType,
			GL_FALSE,
			static_cast<GLsizei>(vertexBuffer.getLayout().getStribe()),
			reinterpret_cast<const void*>(currentElement.offset)
		);
		++mElementsCount;
	}
}

void jengine::JVertexArray::setIndexBuffer(const JIndexBuffer& indexBuffer)
{
	bind();
	indexBuffer.bind();
	mIndicesCount = indexBuffer.getCount();
}

void jengine::JVertexArray::bind() const
{
	glBindVertexArray(mId);
}

void jengine::JVertexArray::unbind()
{
	glBindVertexArray(0);
}
