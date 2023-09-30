#ifndef MGL_RENDERER_H
#define MGL_RENDERER_H

#include <glad/glad.h>
#include <cstdio>
#include <cstdint>
#include <array>

#include "Constants.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

namespace mgl
{
	// Forward declerations
	class Texture;
	struct Rect;
	struct Rectf;

	class Renderer
	{
	private:
		int m_Width;
		int m_Height;
		uint32_t m_Stride = 9 * sizeof(float);

	public:
		Renderer(const int width, const int height);
		~Renderer();

	public:
		void Render(const VertexArray* vertexArray, const IndexBuffer* indexBuffer, const Shader* shader) const;
		void Render(const VertexArray* vertexArray, const Shader* shader, uint32_t count, uint32_t offset = 0) const;
		void RenderLine(const VertexArray* vertexArray, const IndexBuffer* indexBuffer, const Shader* shader) const;
		void Clear() const;
		void SetClearColour(const float r, const float g, const float b, const float a) const;
	};
}

#endif // MGL_RENDERER_H
