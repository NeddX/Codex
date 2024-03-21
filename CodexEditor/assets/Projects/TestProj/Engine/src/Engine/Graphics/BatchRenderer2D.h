#ifndef CODEX_RENDERER_SPRITE_BATCH_RENDERER_H
#define CODEX_RENDERER_SPRITE_BATCH_RENDERER_H

#include <sdafx.h>

#include "../Core/Geomtryd.h"
#include "../Scene/Sprite.h"
#include "RenderBatch.h"
#include "Shader.h"
#include "Texture2D.h"

namespace codex::graphics {
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
        static Shader*                    GetShader() noexcept;
        static usize                      GeBatchCount() noexcept;
        static usize                      GetQuadCount() noexcept;
        static std::vector<RenderBatch*>& GetBatches() noexcept;

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
            RenderRect(sprite.GetTexture().Get(), tex_coords, transform, ncsprite.GetColour(), ncsprite.GetZIndex(),
                       entityId);
        }
    };
} // namespace codex::graphics

#endif // CODEX_RENDERER_SPRITE_BATCH_RENDERER_H
