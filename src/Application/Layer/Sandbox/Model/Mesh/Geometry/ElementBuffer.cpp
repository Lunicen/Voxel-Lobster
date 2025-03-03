#include "ElementBuffer.h"

ElementBuffer::ElementBuffer(const std::vector<TriangleIndexes>& indices)
{
	const auto verticesDataSize = static_cast<GLsizeiptr>(indices.size()) * static_cast<GLsizeiptr>(sizeof(TriangleIndexes));

	glGenBuffers(1, &_bufferRef);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bufferRef);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, verticesDataSize, indices.data(), GL_STATIC_DRAW);
}

void ElementBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bufferRef);
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void ElementBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

ElementBuffer::~ElementBuffer()
{
	glDeleteBuffers(1, &_bufferRef);
}
