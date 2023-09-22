#include "VertexArray.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace mgl
{
	VertexArray::VertexArray()
	{
		GL_Call(glGenVertexArrays(1, &m_RendererId));
		GL_Call(glBindVertexArray(m_RendererId));
	}

	VertexArray::~VertexArray()
	{
		GL_Call(glBindVertexArray(0));
		GL_Call(glDeleteVertexArrays(1, &m_RendererId));
	}

	void VertexArray::Bind() const
	{
		GL_Call(glBindVertexArray(m_RendererId));
	}

	void VertexArray::Unbind() const
	{
		GL_Call(glBindVertexArray(0));
	}

	void VertexArray::AddBuffer(const VertexBuffer* vbo, const VertexBufferLayout* layout) const
	{
		Bind();
		vbo->Bind();
		const auto& elements = layout->GetElements();
		uint32_t stride = layout->GetStride();
		uint32_t offset = 0;
		for (uint32_t i = 0; i < elements.size(); ++i)
		{
			GL_Call(glVertexAttribPointer(
				i, 
				elements[i].count, 
				elements[i].type, 
				elements[i].normalized,
				stride,
				(const void*)offset));
			GL_Call(glEnableVertexAttribArray(i)); // I KEEP FUCKING FORGETING THIS HOLY SHIT
			offset += VertexBufferElement::GetSizeOfType(elements[i].type) * elements[i].count;
		}
	}
}
