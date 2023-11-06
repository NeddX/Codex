#include "RenderBatch.h"

namespace codex {
    RenderBatch::RenderBatch(const i32 maxQuadCount, Shader* shader) : m_MaxQuadCount(maxQuadCount), m_Shader(shader)
    {
        // TODO: Update MGL
        m_MaxTextureSlotCount = 16; // mgl::GraphicsCapabilities::GetMaxTextureSlotCount();

        m_Verticies = new QuadVertex[maxQuadCount];
        m_VertexPtr = m_Verticies;
        m_TextureList.resize(m_MaxTextureSlotCount);

        m_Vao = std::make_unique<mgl::VertexArray>();
        m_Vao->Bind();

        m_Vbo = std::make_unique<mgl::VertexBuffer>();
        m_Vbo->Bind();
        m_Vbo->SetBuffer<f32>(nullptr, QUAD2D_VERTEX_SIZE * sizeof(f32) * maxQuadCount,
                              mgl::BufferUsage::DYNAMIC_DRAW); // you basically allocate space and then upload the data

        m_Ebo = std::make_unique<mgl::IndexBuffer>();
        m_Ebo->Bind();
        u32  size              = 0;
        auto index_buffer_data = GenerateIndicies(size);
        m_Ebo->SetBuffer(index_buffer_data.data(), size);

        m_Layout = std::make_unique<mgl::VertexBufferLayout>();
        m_Layout->Push<f32>(4); // a_Colour
        m_Layout->Push<f32>(3); // a_Position
        m_Layout->Push<f32>(2); // a_TexCoord
        m_Layout->Push<f32>(2); // a_TexDim
        m_Layout->Push<i32>(1); // a_TexId
        m_Layout->Push<i32>(1); // a_EntityId

        m_Vao->AddBuffer(m_Vbo.get(), m_Layout.get());

        // For some weird reason this does not work.
        /*i32 textures[] = {0, 1, 2, 3, 4, 5, 6, 7};
        m_Shader->SetUniform1iArr("u_Textures", 8, textures);*/

        m_QuadCount       = 0;
        m_HasRoom         = true;
        m_CurrentTexIndex = 0;
        std::fill(m_TextureList.begin(), m_TextureList.end(), nullptr);
    }

    RenderBatch::~RenderBatch()
    {
        delete[] m_Verticies;
    }

    void RenderBatch::Flush()
    {
        static bool texture_slots_initialized = false;
        m_QuadCount                           = 0;
        m_HasRoom                             = true;
        m_CurrentTexIndex                     = 0;
        m_VertexPtr                           = m_Verticies;
        std::fill(m_TextureList.begin(), m_TextureList.end(), nullptr);

        if (!texture_slots_initialized)
        {
            std::vector<i32> textures(m_MaxTextureSlotCount);
            for (u32 i = 0; i < (u32)textures.size(); ++i)
                textures[i] = i;
            m_Shader->SetUniform1iArr("u_Textures", 8, textures.data());
            texture_slots_initialized = true;
        }
    }

    bool RenderBatch::UploadQuad(Texture2D* texture, const Rectf& srcRect, const Matrix4f& transform,
                                 const Vector4f& colour, const i32 entityId)
    {
        u16 tex_id     = 0;
        f32 tex_width  = 0.0f;
        f32 tex_height = 0.0f;
        if (texture)
        {
            const auto it = std::find(m_TextureList.begin(), m_TextureList.end(), texture);
            if (it == m_TextureList.end())
            {
                if (m_CurrentTexIndex < m_MaxTextureSlotCount)
                {
                    m_TextureList[m_CurrentTexIndex++] = texture;
                    tex_id                             = m_CurrentTexIndex;
                }
                else
                    return false;
            }
            else
                tex_id = (u16)std::distance(m_TextureList.begin(), it) + 1;
            tex_width  = (f32)texture->GetWidth();
            tex_height = (f32)texture->GetHeight();
        }

        Vector4f quad_verticies[4] = { { 0.5f, 0.5f, 0.0f, 1.0f },
                                       { -0.5f, 0.5f, 0.0f, 1.0f },
                                       { -0.5f, -0.5f, 0.0f, 1.0f },
                                       { 0.5f, -0.5f, 0.0f, 1.0f } };
        Vector2f tex_coords[4]     = { { srcRect.x + srcRect.w, srcRect.y + srcRect.h },
                                       { srcRect.x, srcRect.y + srcRect.h },
                                       { srcRect.x, srcRect.y },
                                       { srcRect.x + srcRect.w, srcRect.y } };

        for (usize i = 0; i < QUAD2D_VERTEX_COUNT; ++i)
        {
            m_VertexPtr->vertex   = transform * quad_verticies[i];
            m_VertexPtr->colour   = colour;
            m_VertexPtr->texCoord = tex_coords[i];
            m_VertexPtr->texId    = tex_id;
            m_VertexPtr->texSize  = { tex_width, tex_height };
            m_VertexPtr->entityId = entityId;
            ++m_VertexPtr;
        }

        if (++m_QuadCount >= m_MaxQuadCount)
            m_HasRoom = false;

        return true;
    }

    std::vector<u32> RenderBatch::GenerateIndicies(u32& size)
    {
        size = 6 * m_MaxQuadCount;
        std::vector<u32> index_buffer_data(size);

        for (usize i = 0; i < size; i += 6)
        {
            u32 offset = (i32)(4 * (i / 6));

            // TODO: Consider using std::memcpy here.
            index_buffer_data[i]     = 0 + offset;
            index_buffer_data[i + 1] = 1 + offset;
            index_buffer_data[i + 2] = 2 + offset;

            index_buffer_data[i + 3] = 2 + offset;
            index_buffer_data[i + 4] = 3 + offset;
            index_buffer_data[i + 5] = 0 + offset;
        }

        return index_buffer_data;
    }

    void RenderBatch::Render()
    {
        m_Shader->Bind();
        m_Vao->Bind();
        m_Ebo->Bind();
        m_Vbo->Bind();
        m_Vbo->SetBufferSubData<QuadVertex>(m_Verticies, 0, sizeof(QuadVertex) * QUAD2D_VERTEX_COUNT * m_QuadCount);
        // m_Vbo->SetBufferSubData<f32>(m_Verticies, 0, QUAD2D_VERTEX_SIZE * m_QuadCount * sizeof(f32));

        for (i32 i = 0; i < m_CurrentTexIndex; ++i)
            m_TextureList[i]->Bind(i);

        GL_Call(glDrawElements(GL_TRIANGLES, 6 * m_QuadCount, GL_UNSIGNED_INT, nullptr));

        m_Shader->Unbind();
        m_Vao->Unbind();
        m_Vbo->Unbind();
        m_Ebo->Unbind();

        for (i32 i = 0; i < m_CurrentTexIndex; ++i)
            m_TextureList[i]->Unbind();
    }
} // namespace codex
