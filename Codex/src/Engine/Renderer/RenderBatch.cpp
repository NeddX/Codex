#include "RenderBatch.h"

namespace Codex
{
	RenderBatch::RenderBatch(int maxQuadCount, Shader* shader) :
		m_MaxQuadCount(maxQuadCount), m_Shader(shader)
	{
		// TODO: Update MARFX
		m_MaxTextureSlotCount = 32;//mgl::GraphicsCapabilities::GetMaxTextureSlotCount();

		m_Verticies = new float[maxQuadCount * VERTEX_SIZE];
		m_TextureList.resize(m_MaxTextureSlotCount);

		m_Vao = std::unique_ptr<mgl::VertexArray>(new mgl::VertexArray());
		m_Vao->Bind();

		m_Vbo = std::unique_ptr<mgl::VertexBuffer>(new mgl::VertexBuffer());
		m_Vbo->Bind();
		m_Vbo->SetBuffer<float>(nullptr, VERTEX_SIZE * sizeof(float) * maxQuadCount, BufferUsage::DYNAMIC_DRAW); // you basically allocate space and then upload the data

		m_Ebo = std::unique_ptr<mgl::IndexBuffer>(new mgl::IndexBuffer());
		m_Ebo->Bind();
		size_t size = 0;
		uint32_t* index_buffer_data = GenerateIndicies(size);
		m_Ebo->SetBuffer(index_buffer_data, size);

		m_Layout = std::unique_ptr<mgl::VertexBufferLayout>(new mgl::VertexBufferLayout());
		m_Layout->Push<float>(3);
		m_Layout->Push<float>(4);
		m_Layout->Push<float>(2);
		m_Layout->Push<float>(1);
		m_Layout->Push<float>(2);

		m_Vao->AddBuffer(m_Vbo.get(), m_Layout.get());

		// For some weird reason this does not work.
		/*int32_t textures[] = {0, 1, 2, 3, 4, 5, 6, 7};
		m_Shader->SetUniform1iArr("u_Textures", 8, textures);*/

		m_QuadCount = 0;
		m_HasRoom = true;
		m_CurrentTexIndex = 0;
		std::fill(m_TextureList.begin(), m_TextureList.end(), nullptr);

		delete index_buffer_data;
	}

	RenderBatch::~RenderBatch()
	{
		delete[] m_Verticies;
	}
}