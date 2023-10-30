#ifndef CODEX_RENDERER_H
#define CODEX_RENDERER_H

#include <sdafx.h>

#include "../Core/Geomtryd.h"
#include "../Scene/Sprite.h"
#include "BatchRenderer2D.h"
#include "Shader.h"

namespace codex {
    // TODO: This is bad... fix it please.
    class Renderer
    {
    private:
        static i32            m_Width;
        static i32            m_Height;
        static mgl::Renderer* m_InternalRenderer;

    public:
        Renderer() = default;

    public:
        static void Init(const i32 width, const i32 height);
        static void Destroy();

    public:
        static inline void Clear() { m_InternalRenderer->Clear(); }
        static inline void SetClearColour(const f32 r, const f32 g, const f32 b, const f32 a)
        {
            m_InternalRenderer->SetClearColour(r, g, b, a);
        }
        static inline void Render(const mgl::VertexArray* vertexArray, const mgl::IndexBuffer* indexBuffer,
                                  const mgl::Shader* shader)
        {
            m_InternalRenderer->Render(vertexArray, indexBuffer, shader);
        }
    };
} // namespace codex

#endif // CODEX_RENDERER_H
