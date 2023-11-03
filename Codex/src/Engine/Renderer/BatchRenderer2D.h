#ifndef CODEX_RENDERER_SPRITE_BATCH_RENDERER_H
#define CODEX_RENDERER_SPRITE_BATCH_RENDERER_H

#include <sdafx.h>

#include "../Core/Geomtryd.h"
#include "../Scene/Sprite.h"
#include "RenderBatch.h"
#include "Shader.h"
#include "Texture2D.h"

namespace codex {
    constexpr auto BATCH_RENDERER_INITIAL_CAPACITY         = 16;
    constexpr auto BATCH_RENDERER_MAX_QUAD_COUNT_PER_BATCH = 1024;

    // Forward decelerations
    class Renderer;

    class BatchRenderer2D
    {
        friend class Renderer;

    private:
        static i32                       m_Capacity;
        static i32                       m_MaxQuadCountPerBatch;
        static Shader*                   m_Shader;
        static std::vector<RenderBatch*> m_Batches;

    private:
        BatchRenderer2D()                                            = default;
        BatchRenderer2D(const BatchRenderer2D& other)                = delete;
        BatchRenderer2D& operator=(const BatchRenderer2D& other)     = delete;
        BatchRenderer2D(BatchRenderer2D&& other) noexcept            = delete;
        BatchRenderer2D& operator=(BatchRenderer2D&& other) noexcept = delete;
        ~BatchRenderer2D()                                           = default;

    public:
        static inline Shader* GetShader() noexcept { return m_Shader; }
        static inline usize   GeBatchCount() noexcept { return m_Batches.size(); }
        static inline usize   GetQuadCount() noexcept
        {
            usize quad_count = 0;
            for (const auto b : m_Batches)
                quad_count += b->GetCount();
            return quad_count;
        };
        static inline std::vector<RenderBatch*>& GetBatches() noexcept { return m_Batches; }

    public:
        static void Init();
        static void Destroy();
        static void Begin();
        static void End();
        static void BindShader(Shader* shader);
        static void RenderRect(Texture2D* texture, const Rectf& srcRect, const Matrix4f& mat, const Vector4f& colour,
                               const i32 zIndex = 0, const i32 entityId = -1);

    public:
        static inline void RenderSprite(const Sprite& sprite, const Matrix4f& transform, const i32 entityId = -1)
        {
            auto& ncsprite   = (Sprite&)sprite;
            auto& tex_coords = ncsprite.GetTextureCoords();
            RenderRect(sprite.GetTexture().get(), tex_coords, transform, ncsprite.GetColour(), ncsprite.GetZIndex(),
                       entityId);
        }
    };
} // namespace codex

#endif // CODEX_RENDERER_SPRITE_BATCH_RENDERER_H
