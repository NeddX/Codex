#include "RenderBatch.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

namespace mfx
{
	RenderBatch2D::RenderBatch2D(int maxBatchSize, Shader* shader) :
		m_MaxBatchSize(maxBatchSize), m_Shader(shader)
	{
		m_Verticies = new float[maxBatchSize * (4 * 10)];
	}

	RenderBatch2D::~RenderBatch2D()
	{
		m_Shader->Unbind();
		m_Ebo->Unbind();
		m_Vbo->Unbind();
		m_Vao->Unbind();

		delete m_Shader;
		delete m_Ebo;
		delete m_Vbo;
		delete m_Vao;
		delete m_Layout;
		delete m_Verticies;
	}

	uint32_t* RenderBatch2D::GenerateIndicies(size_t& size)
	{
		size = 6 * m_MaxBatchSize;
		uint32_t* index_buffer_data = new uint32_t[size];

		for (int i = 0; i < size; i += 6)
		{
			int offset = 4 * (i / 6);

			index_buffer_data[i]		= 2 + offset;
			index_buffer_data[i + 1]	= 0 + offset;
			index_buffer_data[i + 2]	= 1 + offset;
										    
			index_buffer_data[i + 3]	= 2 + offset;
			index_buffer_data[i + 4]	= 3 + offset;
			index_buffer_data[i + 5]	= 1 + offset;
		}

		return index_buffer_data;
	}

	void RenderBatch2D::RenderBegin()
	{
		GL_Call(glClear(GL_COLOR_BUFFER_BIT));

		m_Vao = new VertexArray();
		m_Vao->Bind();

		m_Vbo = new VertexBuffer();
		m_Vbo->Bind();
		m_Vbo->SetBuffer<float>(nullptr, 0, BufferUsage::DYNAMIC_DRAW); // you basically allocate space and then upload the data

		m_Ebo = new IndexBuffer();
		m_Ebo->Bind();
		size_t size = 0;
		uint32_t* index_buffer_data = GenerateIndicies(size);
		m_Ebo->SetBuffer(index_buffer_data, size);

		m_Layout = new VertexBufferLayout();
		m_Layout->Push<float>(3);
		m_Layout->Push<float>(4);
		m_Layout->Push<float>(2);
		m_Layout->Push<float>(1);

		delete index_buffer_data;
	}

	void RenderBatch2D::UploadRect(Texture* texture, const Rectf& srcRect, const Rectf& destRect)
	{
		float vertex_buffer_data[] =
		{
			destRect.x, 					destRect.y,					0.0f,				1.0f, 1.0f, 1.0f, 1.0f,					srcRect.x, 				srcRect.y + srcRect.h,				(float)(0),//texture->GetSlot()),
			destRect.x + destRect.w,		destRect.y,					0.0f,				1.0f, 1.0f, 1.0f, 1.0f,					srcRect.x + srcRect.w,	srcRect.y + srcRect.h,				(float)(0),//texture->GetSlot()),
			destRect.x,						destRect.y + destRect.h,	0.0f,				1.0f, 1.0f, 1.0f, 1.0f,					srcRect.x,				srcRect.y,							(float)(0),//texture->GetSlot()),
			destRect.x + destRect.w,		destRect.y + destRect.h,	0.0f,				1.0f, 1.0f, 1.0f, 1.0f,					srcRect.x + srcRect.w,	srcRect.y,							(float)(0) //texture->GetSlot())
		};

		size_t offset = m_QuadCount * (4 * 10);
		for (int i = 0; i < sizeof(vertex_buffer_data) / sizeof(float); ++i)
			m_Verticies[offset + i] = vertex_buffer_data[i];
		
		if (++m_QuadCount >= m_MaxBatchSize)
			m_HasRoom = false;
	}

	void RenderBatch2D::RenderEnd()
	{
		m_Vao->Bind();

		m_Vbo->Bind();
		m_Vbo->SetBuffer<float>(m_Verticies, 4 * 10 * m_QuadCount * sizeof(float), BufferUsage::DYNAMIC_COPY);
		//m_Vbo->SetBufferSubData<float>(m_Verticies.data(), m_Verticies.size() * sizeof(float));

		m_Vao->AddBuffer(m_Vbo, m_Layout);

		m_Shader->Bind();

		GL_Call(glDrawElements(GL_TRIANGLES, m_QuadCount * 6, GL_UNSIGNED_INT, (const void*)(0)));

		// Reset
		m_Vao->Unbind();
		m_Ebo->Unbind();
		m_Shader->Unbind();
		m_QuadCount = 0;
	}
}
