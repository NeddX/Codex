#ifndef CODEX_RENDERER_SPRITE_BATCH_RENDERER_H
#define CODEX_RENDERER_SPRITE_BATCH_RENDERER_H

#include <sdafx.h>

#include "Shader.h"
#include "Texture2D.h"
#include "RenderBatch.h"
#include "../Core/Geomtryd.h"

namespace codex {
	constexpr auto BATCH_RENDERER_INITIAL_CAPACITY = 16;
	constexpr auto BATCH_RENDERER_MAX_QUAD_COUNT_PER_BATCH = 1024;

	// Forward decelerations
	class Renderer;

	class SpriteBatchRenderer
	{
		friend class Renderer;

	private:
		i32 m_Capacity;
		i32 m_MaxQuadCountPerBatch;
		Shader* m_Shader;
		std::vector<RenderBatch*> m_Batches;

	public:
		SpriteBatchRenderer(
			Shader* shader,
			const i32 capacity = BATCH_RENDERER_INITIAL_CAPACITY, 
			const i32 maxQuadCount = BATCH_RENDERER_MAX_QUAD_COUNT_PER_BATCH);
		SpriteBatchRenderer(const SpriteBatchRenderer& other) = delete;
		SpriteBatchRenderer& operator=(const SpriteBatchRenderer& other) = delete;
		SpriteBatchRenderer(SpriteBatchRenderer&& other) noexcept = delete;
		SpriteBatchRenderer& operator=(SpriteBatchRenderer&& other) noexcept = delete;
		~SpriteBatchRenderer();

	public:
        inline Shader* GetShader() const { return m_Shader; }

    public:
		void Begin();
		void End();
		void BindShader(Shader* shader);
		void RenderRect(
			Texture2D* texture,
			const Rectf& srcRect,
			const Rectf& destRect,
			const Vector4f colour = Vector4f(1.0f, 1.0f, 1.0f, 1.0f),
			const i32 zIndex = 0,
			const i32 entityId = -1);
	};
}

#endif // CODEX_RENDERER_SPRITE_BATCH_RENDERER_H
