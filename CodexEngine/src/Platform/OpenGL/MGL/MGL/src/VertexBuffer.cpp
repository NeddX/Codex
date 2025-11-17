#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Renderer.h"

namespace mgl
{
	VertexBuffer::VertexBuffer()
	{
		GL_Call(glGenBuffers(1, &m_RendererId));
		GL_Call(glBindBuffer(GL_ARRAY_BUFFER, m_RendererId));
	}

	VertexBuffer::~VertexBuffer()
	{
		GL_Call(glBindBuffer(GL_ARRAY_BUFFER, m_RendererId));
		glDeleteBuffers(1, &m_RendererId);
	}

	void VertexBuffer::Bind() const
	{
		GL_Call(glBindBuffer(GL_ARRAY_BUFFER, m_RendererId));
	}

	void VertexBuffer::Unbind() const
	{
		GL_Call(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
}