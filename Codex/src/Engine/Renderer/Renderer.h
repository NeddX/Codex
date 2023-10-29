#ifndef CODEX_RENDERER_H
#define CODEX_RENDERER_H

#include <sdafx.h>

#include "../Core/Geomtryd.h"
#include "Shader.h"
#include "SpriteBatchRenderer.h"

namespace codex {
    class Renderer
    {
    private:
        i32                                  m_Width;
        i32                                  m_Height;
        std::unique_ptr<mgl::Renderer>       m_InternalRenderer;
        std::unique_ptr<SpriteBatchRenderer> m_BatchRenderer;

    public:
        Renderer(const i32 width, const i32 height);

    public:
        inline void Clear() const { m_InternalRenderer->Clear(); }
        inline void SetClearColour(const f32 r, const f32 g, const f32 b, const f32 a) const
        {
            m_InternalRenderer->SetClearColour(r, g, b, a);
        }
        inline SpriteBatchRenderer* GetSpriteBatchRenderer(
            Shader* shader, const i32 initialBatchCapacity = BATCH_RENDERER_INITIAL_CAPACITY,
            const i32 maxBatchQuadCount = BATCH_RENDERER_MAX_QUAD_COUNT_PER_BATCH)
        {
            if (!m_BatchRenderer)
                m_BatchRenderer =
                    std::make_unique<SpriteBatchRenderer>(shader, initialBatchCapacity, maxBatchQuadCount);
            return m_BatchRenderer.get();
        }
        inline void Render(const mgl::VertexArray* vertexArray, const mgl::IndexBuffer* indexBuffer,
                           const mgl::Shader* shader) const
        {
            m_InternalRenderer->Render(vertexArray, indexBuffer, shader);
        }
    };
} // namespace codex

#endif // CODEX_RENDERER_H
