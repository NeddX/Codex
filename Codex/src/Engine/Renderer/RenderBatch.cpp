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
        m_Layout->Push<f32>(3); // a_Position
        m_Layout->Push<f32>(4); // a_Colour
        m_Layout->Push<f32>(2); // a_TexCoord
        m_Layout->Push<f32>(1); // a_TexId
        m_Layout->Push<f32>(2); // a_TexDim
        m_Layout->Push<f32>(1); // a_EntityId

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
} // namespace codex
