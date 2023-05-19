#ifndef CODEX_RENDERER_H
#define CODEX_RENDERER_H

#include <sdafx.h>

#include "Shader.h"
#include "../Core/Geomtryd.h"
#include "SpriteBatchRenderer.h"

namespace Codex {
	class Renderer
	{
#ifdef CDX_DEBUG_CUSTOM_ALLOCATORS
	public:
		void* operator new(size_t size)
		{
			void* ptr = std::malloc(size);
			fmt::println("[Memory] :: Allocated memory.\n\tFile: {}\n\tLine: {}\n\tSize: {}\n\tAddress: {}",
				__FILE__, __LINE__, size, ptr);
			return ptr;
		}
		void operator delete(void* ptr)
		{
			fmt::println("[Memory] :: Deallocated memory.\n\tFile: {}\n\tLine: {}\n\tAddress: {}", __FILE__, __LINE__, ptr);
			std::free(ptr);
		}
#endif

	private:
		int m_Width;
		int m_Height;
		std::unique_ptr<mgl::Renderer> m_InternalRenderer;
		std::shared_ptr<SpriteBatchRenderer> m_BatchRenderer;

	public:
		Renderer(const int width, const int height);
		~Renderer();

	public:
		inline void Clear()																		const { m_InternalRenderer->Clear(); }
		inline void SetClearColour(const float r, const float g, const float b, const float a)	const { m_InternalRenderer->SetClearColour(r, g, b, a); }
		inline std::shared_ptr<SpriteBatchRenderer> GetSpriteBatchRenderer(
			Shader* shader,
			const int initialBatchCapacity	= BATCH_RENDERER_INITIAL_CAPACITY, 
			const int maxBatchQuadCount		= BATCH_RENDERER_MAX_QUAD_COUNT_PER_BATCH)
		{
			if (!m_BatchRenderer)
				m_BatchRenderer = std::make_shared<SpriteBatchRenderer>(shader, initialBatchCapacity, maxBatchQuadCount);
			return m_BatchRenderer;
		}
		inline void Render(const mgl::VertexArray* vertexArray, const mgl::IndexBuffer* indexBuffer, const mgl::Shader* shader) const { m_InternalRenderer->Render(vertexArray, indexBuffer, shader); }
	};
}

#endif // CODEX_RENDERER_H