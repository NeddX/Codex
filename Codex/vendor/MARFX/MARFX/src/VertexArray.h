#ifndef MARFX_VERTEX_ARRAY_H
#define MARFX_VERTEX_ARRAY_H

#include <cstdint>

namespace mfx
{
	// Forward declerations
	class VertexBuffer;
	class VertexBufferLayout;
	
	class VertexArray
	{
	private:
		uint32_t m_RendererID;

	public:
		VertexArray();
		~VertexArray();

	public:
		void Bind() const;
		void Unbind() const;
		void AddBuffer(const VertexBuffer* vbo, const VertexBufferLayout* layout) const;
	};
}

#endif // MARFX_VERTEX_ARRAY_H