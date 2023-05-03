#ifndef MARFX_RENDER_BATCH_2D_H
#define MARFX_RENDER_BATCH_2D_H

#include <vector>
#include <memory>
#include <array>

#include "Geometry.h"

namespace mfx
{
	// Forward declerations
	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;
	class VertexBufferLayout;
	class Shader;
	class Texture;

	class RenderBatch2D
	{
	private:
		int m_QuadCount = 0;
		const int m_MaxBatchSize = 0;
		const int m_VertexCount = m_MaxBatchSize * (10 * 4);
		bool m_HasRoom = true;
		const char* m_ShaderFile;
		float* m_Verticies;
		VertexArray* m_Vao;
		VertexBuffer* m_Vbo;
		IndexBuffer* m_Ebo;
		VertexBufferLayout* m_Layout;
		Shader* m_Shader;

	public:
		RenderBatch2D(int maxBatchSize, Shader* shader);
		~RenderBatch2D();

	public:
		uint32_t* GenerateIndicies(size_t& size);
		void RenderBegin(); // Start()
		void UploadRect(Texture* texture, const Rectf& srcRect, const Rectf& destRect); // Add Sprite
		void RenderEnd(); // Render
	};
}

#endif // MARFX_RENDER_BATCH_H