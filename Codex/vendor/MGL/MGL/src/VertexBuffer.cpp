#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Renderer.h"

namespace mgl
{
	VertexBuffer::VertexBuffer()
	{
		GL_Call(glGenBuffers(1, &m_RendererID));
		GL_Call(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	}

	VertexBuffer::~VertexBuffer()
	{
		GL_Call(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
		glDeleteBuffers(1, &m_RendererID);
	}

	void VertexBuffer::Bind() const
	{
		GL_Call(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	}

	void VertexBuffer::Unbind() const
	{
		GL_Call(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
}