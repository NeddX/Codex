#ifndef CODEX_RENDERER_RENDER_BATCH_H
#define CODEX_RENDERER_RENDER_BATCH_H

#include <sdafx.h>

#include "Shader.h"
#include "Texture2D.h"
#include "../Core/Geomtryd.h"

namespace codex {
	constexpr auto QUAD2D_VERTEX_COMPONENT_COUNT = 13;											// How many components does a vertex have?
	constexpr auto QUAD2D_VERTEX_COUNT = 4;														// How many vertices does the buffer have?
	constexpr auto QUAD2D_VERTEX_SIZE = QUAD2D_VERTEX_COUNT * QUAD2D_VERTEX_COMPONENT_COUNT;	// The total count of the elements in the buffer

	class RenderBatch
	{
#ifdef CX_DEBUG_CUSTOM_ALLOCATORS
	public:
		void* operator new(usize size)
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
#endif

	private:
		i32 m_QuadCount = 0;
		i32 m_MaxTextureSlotCount = 32;
		i32 m_MaxQuadCount = 1000;
		i32 m_ZIndex = 0;
		bool m_HasRoom = true;
		std::vector<f32> m_Verticies;
		std::unique_ptr<mgl::VertexArray> m_Vao;
		std::unique_ptr<mgl::VertexBuffer> m_Vbo;
		std::unique_ptr<mgl::IndexBuffer> m_Ebo;
		std::unique_ptr<mgl::VertexBufferLayout> m_Layout;
		Shader* m_Shader;
		std::vector<Texture2D*> m_TextureList;
		u16 m_CurrentTexIndex;

	public:
		RenderBatch(const i32 maxQuadCount, Shader* shader);
		~RenderBatch();

	public:
		inline bool HasRoom() const
			{ return m_HasRoom; }
		inline i32 GetZIndex() const
			{ return m_ZIndex; }
		inline i32 GetCount() const
			{ return m_QuadCount; }
		inline void SetZIndex(i32 newIndex)
			{ m_ZIndex = newIndex; }

	public:
		inline void BindShader(Shader* shader)
			{ m_Shader = shader; }
		inline void Flush()
		{
			static bool texture_slots_initialized = false;
			m_QuadCount = 0;
			m_HasRoom = true; 
			m_CurrentTexIndex = 0;
			std::fill(m_TextureList.begin(), m_TextureList.end(), nullptr);

			if (!texture_slots_initialized)
			{
				std::vector<i32> textures(m_MaxTextureSlotCount);
				for (i32 i = 0; i < textures.size(); ++i) textures[i] = i;
				m_Shader->SetUniform1iArr("u_Textures", 8, textures.data());
				texture_slots_initialized = true;
			}
		}
		inline bool UploadQuad(
			Texture2D* texture,
			const Rectf& srcRect,
			const Rectf& destRect,
			const Vector4f colour,
			const i32 entityId)
		{
			u16 tex_id = 0;
			f32 tex_width = 0.0f;
			f32 tex_height = 0.0f;
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
				else tex_id = (u16)std::distance(m_TextureList.begin(), it) + 1;
				tex_width	= (f32)texture->GetWidth();
				tex_height	= (f32)texture->GetHeight();
			}

			f32 vertex_buffer_data[] =
			{
				destRect.x, 					destRect.y,					0.0f,				colour.x, colour.y, colour.z, colour.w,					srcRect.x,				srcRect.y,						(f32)tex_id,		tex_width, tex_height,		(f32)entityId,
				destRect.x + destRect.w,		destRect.y,					0.0f,				colour.x, colour.y, colour.z, colour.w,					srcRect.x + srcRect.w,	srcRect.y,						(f32)tex_id,		tex_width, tex_height,		(f32)entityId,
				destRect.x,						destRect.y + destRect.h,	0.0f,				colour.x, colour.y, colour.z, colour.w,					srcRect.x,				srcRect.y + srcRect.h,			(f32)tex_id,		tex_width, tex_height,		(f32)entityId,	
				destRect.x + destRect.w,		destRect.y + destRect.h,	0.0f,				colour.x, colour.y, colour.z, colour.w,					srcRect.x + srcRect.w,	srcRect.y + srcRect.h,			(f32)tex_id,		tex_width, tex_height,		(f32)entityId
			};

			// TODO: Consider directly adding the vertex buffer data to m_Verticies instead of creating a temporary
			// array and then copying the contents of it to m_Verticies.

			u32 offset = m_QuadCount * QUAD2D_VERTEX_SIZE;
			std::memcpy(m_Verticies.data() + offset, vertex_buffer_data, sizeof(vertex_buffer_data));

			if (++m_QuadCount >= m_MaxQuadCount)
				m_HasRoom = false;

			return true;
		}
		inline std::vector<u32> GenerateIndicies(u32& size)
		{
			size = 6 * m_MaxQuadCount;
			std::vector<u32> index_buffer_data(size);

			for (usize i = 0; i < size; i += 6)
			{
				u32 offset = (i32)(4 * (i / 6));

				// TODO: Consider using std::memcpy here.
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
			m_Vbo->SetBufferSubData<f32>(m_Verticies.data(), 0, QUAD2D_VERTEX_SIZE * m_QuadCount * sizeof(f32));
			
			for (i32 i = 0; i < m_CurrentTexIndex; ++i)
				m_TextureList[i]->Bind(i);

			glDrawElements(GL_TRIANGLES, 6 * m_QuadCount, GL_UNSIGNED_INT, nullptr);

			m_Shader->Unbind();
			m_Vao->Unbind();
			m_Vbo->Unbind();
			m_Ebo->Unbind();

			for (i32 i = 0; i < m_CurrentTexIndex; ++i)
				m_TextureList[i]->Unbind();
		}
	};
}

#endif // CODEX_RENDERER_RENDER_BATCH_H
