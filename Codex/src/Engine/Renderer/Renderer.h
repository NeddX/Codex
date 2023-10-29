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
        i32                            m_Width;
        i32                            m_Height;
        std::unique_ptr<mgl::Renderer> m_InternalRenderer;

    public:
        Renderer(const i32 width, const i32 height);

    public:
        inline void Clear() const { m_InternalRenderer->Clear(); }
        inline void SetClearColour(const f32 r, const f32 g, const f32 b, const f32 a) const
        {
            m_InternalRenderer->SetClearColour(r, g, b, a);
        }
        inline void Render(const mgl::VertexArray* vertexArray, const mgl::IndexBuffer* indexBuffer,
                           const mgl::Shader* shader) const
        {
            m_InternalRenderer->Render(vertexArray, indexBuffer, shader);
        }
    };
} // namespace codex

#endif // CODEX_RENDERER_H
