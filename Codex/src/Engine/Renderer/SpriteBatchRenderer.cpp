#include "SpriteBatchRenderer.h"
#include "../Core/ResourceHandler.h"

namespace Codex {
	SpriteBatchRenderer::SpriteBatchRenderer(Shader* shader, int capacity, const int maxQuadCount) :
		m_Shader(shader), m_Capacity(capacity), m_MaxQuadCountPerBatch(maxQuadCount)
	{
		m_Batches.reserve(capacity);
		for (int i = capacity; i < capacity; ++i)
			m_Batches.push_back(new RenderBatch(maxQuadCount, m_Shader));
	}

	SpriteBatchRenderer::~SpriteBatchRenderer()
	{

	}

	void SpriteBatchRenderer::Begin()
	{
		std::for_each(m_Batches.begin(), m_Batches.end(),
			[](auto* b)
			{
				if (b->GetCount() > 0)
					b->Flush();
			});
	}
	void SpriteBatchRenderer::End()
	{
		std::vector<RenderBatch*> sorted_batch(m_Batches.begin(), m_Batches.end());
		std::sort(sorted_batch.begin(), sorted_batch.end(), 
			[](auto* a, auto* b)
			{
				return a->GetZIndex() < b->GetZIndex();
			});

		std::for_each(sorted_batch.begin(), sorted_batch.end(),
			[](auto* b)
			{
				if (b->GetCount() > 0)
					b->Render();
			});
	}

	void SpriteBatchRenderer::RenderRect(Texture2D* texture, const Rectf& srcRect, const Rectf& destRect, Vector4f colour, int zIndex)
	{
		for (auto* batch : m_Batches)
		{
			if (batch->HasRoom() && batch->GetZIndex() == zIndex)
			{
				if (batch->UploadQuad(texture, srcRect, destRect, colour)) return;
				else std::cout << "failed to upload quad\n";
			}
		}

		// If there was no space then create a new batch
		auto* new_batch = new RenderBatch(m_MaxQuadCountPerBatch, m_Shader);
		new_batch->SetZIndex(zIndex);
		new_batch->Flush();
		new_batch->UploadQuad(texture, srcRect, destRect, colour);
		m_Batches.push_back(new_batch);
	}
}
