#ifndef MGL_VERTEX_ARRAY_H
#define MGL_VERTEX_ARRAY_H

#include <cstdint>

namespace mgl
{
	// Forward declerations
	class VertexBuffer;
	class VertexBufferLayout;
	
	class VertexArray
	{
	private:
		uint32_t m_RendererId;

	public:
		VertexArray();
		~VertexArray();

	public:
		void Bind() const;
		void Unbind() const;
		void AddBuffer(const VertexBuffer* vbo, const VertexBufferLayout* layout) const;
	};
}

#endif // MGL_VERTEX_ARRAY_H