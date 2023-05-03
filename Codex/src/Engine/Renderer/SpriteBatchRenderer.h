#ifndef CODEX_RENDERER_SPRITE_BATCH_RENDERER_H
#define CODEX_RENDERER_SPRITE_BATCH_RENDERER_H

#include <sdafx.h>

#include "Shader.h"
#include "Texture2D.h"
#include "RenderBatch.h"
#include "../Core/Geomtryd.h"

namespace Codex
{
	constexpr auto BATCH_RENDERER_INITIAL_CAPACITY = 16;
	constexpr auto BATCH_RENDERER_MAX_QUAD_COUNT_PER_BATCH = 1024;

	// Forward decelerations
	class Renderer;

	class SpriteBatchRenderer
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

		friend class Renderer;

	private:
		int m_Capacity;
		int m_MaxQuadCountPerBatch;
		Shader* m_Shader;
		std::vector<RenderBatch*> m_Batches;

	protected:
		SpriteBatchRenderer(
			Shader* shader,
			const int capacity = BATCH_RENDERER_INITIAL_CAPACITY, 
			const int maxQuadCount = BATCH_RENDERER_MAX_QUAD_COUNT_PER_BATCH);
		~SpriteBatchRenderer();

	public:
		void Begin();
		void End();
		void RenderRect(Texture2D* texture, const Rectf& srcRect, const Rectf& destRect, int zIndex = 0);
	};
}

#endif // CODEX_RENDERER_SPRITE_BATCH_RENDERER_H