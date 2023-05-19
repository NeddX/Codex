#ifndef MGL_VERTEX_BUFFER_H
#define MGL_VERTEX_BUFFER_H

#include <cstdint>

#include "Renderer.h"

namespace mgl
{
	class VertexBuffer
	{
	private:
		uint32_t m_RendererID;
		size_t m_VertexCount;

	public:
		VertexBuffer();
		~VertexBuffer();

	public:
		inline size_t GetVertexCount() const { return m_VertexCount; }
		template<typename T>
		void SetBuffer(const void* data, size_t size, BufferUsage usage = BufferUsage::STATIC_DRAW)
		{
			GL_Call(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
			GL_Call(glBufferData(GL_ARRAY_BUFFER, size, data, (GLenum)(usage)));
			m_VertexCount = size / sizeof(T);
		}
		template<typename T>
		void SetBufferSubData(const void* data, size_t offset, size_t size)
		{
			GL_Call(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
			GL_Call(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
			m_VertexCount = size / sizeof(T);
		}

	public:
		void Bind() const;
		void Unbind() const;
	};
}

#endif // MGL_VERTEX_BUFFER_H