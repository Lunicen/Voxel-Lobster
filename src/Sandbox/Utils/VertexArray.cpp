#include "VertexArray.h"


VertexArray::VertexArray()
{
	glGenVertexArrays(1, &_arrayRef);
	Bind();
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void VertexArray::Link(VertexBuffer& vbo, const GLuint layout, const GLint size, const GLsizei stride, GLint offset) //TODO to jest inaczej LinkAttrib!!!
{
	vbo.Bind();
	glVertexAttribPointer(
		layout, size,
		GL_FLOAT, GL_FALSE,
		static_cast<GLsizei>(stride * sizeof(float)),
		reinterpret_cast<const void*>(offset * sizeof(float))
	);

	glEnableVertexAttribArray(layout);
	vbo.Unbind();
}

void VertexArray::Bind() const
{
	glBindVertexArray(_arrayRef);
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void VertexArray::Unbind()
{
	glBindVertexArray(0);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &_arrayRef);
}
