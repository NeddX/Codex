#include "BatchRenderer2D.h"
#include "../Core/ResourceHandler.h"

namespace codex {
    i32                       BatchRenderer2D::m_Capacity             = BATCH_RENDERER_INITIAL_CAPACITY;
    i32                       BatchRenderer2D::m_MaxQuadCountPerBatch = BATCH_RENDERER_MAX_QUAD_COUNT_PER_BATCH;
    Shader*                   BatchRenderer2D::m_Shader               = nullptr;
    std::vector<RenderBatch*> BatchRenderer2D::m_Batches;

    void BatchRenderer2D::Init()
    {
        m_Batches.reserve(m_Capacity);
        for (usize i = 0; i < m_Capacity; ++i)
            m_Batches.push_back(new RenderBatch(m_MaxQuadCountPerBatch, m_Shader));
    }

    void BatchRenderer2D::Destroy()
    {
        for (const auto& b : m_Batches)
            delete b;
    }

    void BatchRenderer2D::Begin()
    {
        std::for_each(m_Batches.begin(), m_Batches.end(),
                      [](auto* b)
                      {
                          if (b->GetCount() > 0)
                              b->Flush();
                      });
    }
    void BatchRenderer2D::End()
    {
        std::vector<RenderBatch*> sorted_batch(m_Batches.begin(), m_Batches.end());
        std::sort(sorted_batch.begin(), sorted_batch.end(),
                  [](auto* a, auto* b) { return a->GetZIndex() < b->GetZIndex(); });

        std::for_each(sorted_batch.begin(), sorted_batch.end(),
                      [](auto* b)
                      {
                          if (b->GetCount() > 0)
                              b->Render();
                      });
    }

    void BatchRenderer2D::BindShader(Shader* shader)
    {
        m_Shader = shader;
        for (auto* batch : m_Batches)
            batch->BindShader(shader);
    }

    void BatchRenderer2D::RenderRect(Texture2D* texture, const Rectf& srcRect, const Rectf& destRect,
                                     const Vector4f& colour, const f32 rotZ, const i32 zIndex, const i32 entityId)
    {
        for (auto* batch : m_Batches)
        {
            if (batch->HasRoom() && batch->GetZIndex() == zIndex)
            {
                if (batch->UploadQuad(texture, srcRect, destRect, colour, entityId))
                    return;
                else
                    std::cout << "failed to upload quad\n";
            }
        }

        // If there was no space then create a new batch
        auto* new_batch = new RenderBatch(m_MaxQuadCountPerBatch, m_Shader);
        new_batch->SetZIndex(zIndex);
        new_batch->Flush();
        new_batch->UploadQuad(texture, srcRect, destRect, colour, entityId);
        m_Batches.push_back(new_batch);
    }
} // namespace codex
