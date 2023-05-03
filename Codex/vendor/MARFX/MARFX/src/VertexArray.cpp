#include "VertexArray.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace mfx
{
	VertexArray::VertexArray()
	{
		GL_Call(glGenVertexArrays(1, &m_RendererID));
		GL_Call(glBindVertexArray(m_RendererID));
	}

	VertexArray::~VertexArray()
	{
		GL_Call(glBindVertexArray(0));
		GL_Call(glDeleteVertexArrays(1, &m_RendererID));
	}

	void VertexArray::Bind() const
	{
		GL_Call(glBindVertexArray(m_RendererID));
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
		size_t stride = layout->GetStride();
		size_t offset = 0;
		for (size_t i = 0; i < elements.size(); ++i)
		{
			GL_Call(glVertexAttribPointer(
				i, 
				elements[i].Count, 
				elements[i].Type, 
				elements[i].Normalized,
				stride,
				(const void*) offset));
			GL_Call(glEnableVertexAttribArray(i)); // I KEEP FUCKING FORGETING THIS HOLY SHIT
			offset += VertexBufferElement::GetSizeOfType(elements[i].Type) * elements[i].Count;
		}
	}
}
