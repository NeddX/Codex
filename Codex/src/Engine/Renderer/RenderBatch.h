#ifndef CODEX_RENDERER_RENDER_BATCH_H
#define CODEX_RENDERER_RENDER_BATCH_H

#include <sdafx.h>

#include "Shader.h"
#include "Texture2D.h"
#include "../Core/Geomtryd.h"

namespace Codex
{
	static constexpr auto VERTEX_COMPONENT_COUNT = 12;							// How many components does a vertex have?
	static constexpr auto VERTEX_COUNT = 4;										// How many verticies does the buffer have?
	static constexpr auto VERTEX_SIZE = VERTEX_COUNT * VERTEX_COMPONENT_COUNT;	// The total count of the elements in the buffer

	class RenderBatch
	{
	public:
		void* operator new(size_t size)
		{
			void* ptr = std::malloc(size);
			fmt::println("[Memory] :: Allocated memory.\n\tFile: {}\n\tLine: {}\n\tSize: {}\n\tAddress: {}",
				__FILE__, __LINE__, size, ptr);
			return ptr;
		}
		void operator delete(void* ptr)
		{
			fmt::println("[Memory] :: Deallocated memory.\n\tFile: {}\n\tLine: {}\n\tAddress: {}", __FILE__, __LINE__, ptr);
			std::free(ptr);
		}

	private:
		int m_QuadCount = 0;
		int m_MaxTextureSlotCount = 8;
		int m_MaxQuadCount = 1000;
		int m_ZIndex = 0;
		bool m_HasRoom = true;
		float* m_Verticies;
		std::unique_ptr<mgl::VertexArray> m_Vao;
		std::unique_ptr<mgl::VertexBuffer> m_Vbo;
		std::unique_ptr<mgl::IndexBuffer> m_Ebo;
		std::unique_ptr<mgl::VertexBufferLayout> m_Layout;
		Shader* m_Shader;
		std::vector<Texture2D*> m_TextureList;
		uint16_t m_CurrentTexIndex;

	public:
		RenderBatch(int maxQuadCount, Shader* shader);
		~RenderBatch();

	public:
		inline bool HasRoom() const			{ return m_HasRoom; }
		inline int GetZIndex() const		{ return m_ZIndex; }
		inline int GetCount() const			{ return m_QuadCount; }
		inline void SetZIndex(int newIndex) { m_ZIndex = newIndex; }

	public:
		inline void Flush()
		{
			static bool texture_slots_initialized = false;
			m_QuadCount = 0;
			m_HasRoom = true; 
			m_CurrentTexIndex = 0;
			std::fill(m_TextureList.begin(), m_TextureList.end(), nullptr);

			if (!texture_slots_initialized)
			{
				std::vector<int32_t> textures(m_MaxTextureSlotCount);
				for (int i = 0; i < textures.size(); ++i) textures[i] = i;
				m_Shader->SetUniform1iArr("u_Textures", 8, textures.data());
				texture_slots_initialized = true;
			}
		}
		inline bool UploadQuad(Texture2D* texture, const Rectf& srcRect, const Rectf& destRect)
		{
			uint16_t tex_id = 0;
			float tex_width = 0.0f;
			float tex_height = 0.0f;
			if (texture)
			{
				const auto it = std::find(m_TextureList.begin(), m_TextureList.end(), texture);
				if (it == m_TextureList.end())
				{
					if (m_CurrentTexIndex < m_MaxTextureSlotCount)
					{
						m_TextureList[m_CurrentTexIndex++] = texture;
						tex_id = m_CurrentTexIndex;
					}
					else return false;
				}
				else tex_id = (uint16_t)(std::distance(m_TextureList.begin(), it));
				tex_width	= (float)(texture->GetWidth());
				tex_height	= (float)(texture->GetHeight());
			}

			float vertex_buffer_data[] =
			{
				destRect.x, 					destRect.y,					0.0f,				1.0f, 1.0f, 1.0f, 1.0f,					srcRect.x,				srcRect.y,						(float)(tex_id),		tex_width, tex_height,
				destRect.x + destRect.w,		destRect.y,					0.0f,				1.0f, 1.0f, 1.0f, 1.0f,					srcRect.x + srcRect.w,	srcRect.y,						(float)(tex_id),		tex_width, tex_height,
				destRect.x,						destRect.y + destRect.h,	0.0f,				1.0f, 1.0f, 1.0f, 1.0f,					srcRect.x,				srcRect.y + srcRect.h,			(float)(tex_id),		tex_width, tex_height,
				destRect.x + destRect.w,		destRect.y + destRect.h,	0.0f,				1.0f, 1.0f, 1.0f, 1.0f,					srcRect.x + srcRect.w,	srcRect.y + srcRect.h,			(float)(tex_id),		tex_width, tex_height
			};

			// TODO: Consider directly adding the vertex buffer data to m_Verticies instead of creating a temporary
			// array and then copying the contents of it to 
			size_t offset = m_QuadCount * VERTEX_SIZE;
			std::memcpy(m_Verticies + offset, vertex_buffer_data, sizeof(vertex_buffer_data));
			/*for (int i = 0; i < sizeof(vertex_buffer_data) / sizeof(float); ++i)
				m_Verticies[offset + i] = vertex_buffer_data[i];*/

			if (++m_QuadCount >= m_MaxQuadCount)
				m_HasRoom = false;

			return true;
		}
		inline uint32_t* GenerateIndicies(size_t& size)
		{
			size = 6 * m_MaxQuadCount;
			uint32_t* index_buffer_data = new uint32_t[size];

			for (int i = 0; i < size; i += 6)
			{
				int offset = 4 * (i / 6);

				index_buffer_data[i] = 2 + offset;
				index_buffer_data[i + 1] = 0 + offset;
				index_buffer_data[i + 2] = 1 + offset;

				index_buffer_data[i + 3] = 2 + offset;
				index_buffer_data[i + 4] = 3 + offset;
				index_buffer_data[i + 5] = 1 + offset;
			}

			return index_buffer_data;
		}
		inline void Render()
		{
			m_Shader->Bind();
			m_Vao->Bind();
			m_Ebo->Bind();
			m_Vbo->Bind();
			m_Vbo->SetBufferSubData<float>(m_Verticies, VERTEX_SIZE * m_MaxQuadCount * sizeof(float));
			
			for (int i = 0; i < m_CurrentTexIndex; ++i)
				m_TextureList[i]->Bind(i);

			glDrawElements(GL_TRIANGLES, 6 * m_QuadCount, GL_UNSIGNED_INT, nullptr);
			m_Shader->Unbind();
			m_Vao->Unbind();
			m_Vbo->Unbind();
			m_Ebo->Unbind();

			for (int i = 0; i < m_CurrentTexIndex; ++i)
				m_TextureList[i]->Unbind();
		}
	};
}

#endif // CODEX_RENDERER_RENDER_BATCH_H