#ifndef MARFX_INDEX_BUFFER_H
#define MARFX_INDEX_BUFFER_H

#include <cstdint>

#include "Constants.h"

namespace mfx
{
	// Forward declerations
	class Renderer;

	class IndexBuffer
	{
		friend class Renderer;

	private:
		uint32_t m_RendererID;
		size_t m_Indicies;

	public:
		IndexBuffer();
		~IndexBuffer();

	public:
		inline size_t GetIndexCount() const { return m_Indicies; }

	public:
		void Bind() const;
		void Unbind() const;
		void SetBuffer(const uint32_t* data, size_t indexCount, BufferUsage usage = BufferUsage::STATIC_DRAW);
		void SetBufferSubData(const uint32_t* data, size_t indexCount);
	};
}

#endif // MARFX_VERTEX_BUFFER_H