#ifndef CODEX_RENDERER_RENDER_BATCH_H
#define CODEX_RENDERER_RENDER_BATCH_H

#include <sdafx.h>

#include "../Core/Geomtryd.h"
#include "Shader.h"
#include "Texture2D.h"

namespace codex {
    constexpr auto QUAD2D_VERTEX_COMPONENT_COUNT = 13; // How many components does a vertex have?
    constexpr auto QUAD2D_VERTEX_COUNT           = 4;  // How many vertices does the buffer have?
    constexpr auto QUAD2D_VERTEX_SIZE =
        QUAD2D_VERTEX_COUNT * QUAD2D_VERTEX_COMPONENT_COUNT; // The total count of the elements in the buffer

    struct QuadVertex
    {
        Vector4f colour;
        Vector3f vertex;
        Vector2f texCoord;
        Vector2f texSize;
        i32      texId;
        i32      entityId;
    };

    class RenderBatch
    {
    private:
        i32                                      m_QuadCount           = 0;
        i32                                      m_MaxTextureSlotCount = 32;
        i32                                      m_MaxQuadCount        = 1000;
        i32                                      m_ZIndex              = 0;
        bool                                     m_HasRoom             = true;
        QuadVertex*                              m_Verticies           = nullptr;
        QuadVertex*                              m_VertexPtr           = nullptr;
        std::unique_ptr<mgl::VertexArray>        m_Vao;
        std::unique_ptr<mgl::VertexBuffer>       m_Vbo;
        std::unique_ptr<mgl::IndexBuffer>        m_Ebo;
        std::unique_ptr<mgl::VertexBufferLayout> m_Layout;
        Shader*                                  m_Shader;
        std::vector<Texture2D*>                  m_TextureList;
        u16                                      m_CurrentTexIndex;

    public:
        RenderBatch(const i32 maxQuadCount, Shader* shader);
        ~RenderBatch();

    public:
        bool        HasRoom() const { return m_HasRoom; }
        i32         GetZIndex() const { return m_ZIndex; }
        i32         GetCount() const { return m_QuadCount; }
        void        SetZIndex(i32 newIndex) { m_ZIndex = newIndex; }
        void        BindShader(Shader* shader) { m_Shader = shader; }
        QuadVertex* GetQuads() noexcept { return m_Verticies; }

    public:
        void Flush();
        bool UploadQuad(Texture2D* texture, const Rectf& srcRect, const Matrix4f& transform, const Vector4f& colour,
                        const i32 entityId);
        std::vector<u32> GenerateIndicies(u32& size);
        void             Render();
    };
} // namespace codex

#endif // CODEX_RENDERER_RENDER_BATCH_H
