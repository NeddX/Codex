#ifndef CODEX_RENDERER_DEBUG_DRAW_H
#define CODEX_RENDERER_DEBUG_DRAW_H

#include <sdafx.h>

#include "Line2D.h"
#include "Shader.h"

namespace codex {
	constexpr auto LINE2D_MAX_LINES = 5000;
	constexpr auto LINE2D_INDEX_COUNT = 6;														// How many indices does a single vertex buffer have
	constexpr auto LINE2D_VERTEX_COMPONENT_COUNT = 7;											// How many components does a vertex have?
	constexpr auto LINE2D_VERTEX_COUNT = 2;														// How many vertices does the buffer have?
	constexpr auto LINE2D_VERTEX_SIZE = LINE2D_VERTEX_COUNT * LINE2D_VERTEX_COMPONENT_COUNT;	// The total count of the elements in the buffer

	class DebugDraw
	{
	private:
		std::unique_ptr<mgl::VertexArray> m_Vao;
		std::unique_ptr<mgl::VertexBuffer> m_Vbo;
		std::unique_ptr<mgl::IndexBuffer> m_Ebo;
		std::unique_ptr<mgl::VertexBufferLayout> m_Layout;
		std::unique_ptr<Shader> m_Shader;

	private:
		static DebugDraw* m_Instance;
		static std::vector<Line2D> m_Lines;
		static std::array<f32, LINE2D_MAX_LINES * 7 * 2> m_Verticies;

	private:
		DebugDraw() = default;
		~DebugDraw() = default;

	public:
		static void Init();
		static void Begin();
		static void Render();
		static void Destroy();

		static void DrawLine2D(
			const Vector2f source,
			const Vector2f destination,
			const Vector4f colour = { 1.0f, 1.0f, 1.0f, 1.0f },
			const i32 lifeTime = 1);
		static void DrawRect2D(
			const Rectf rect,
			const f32 angle = 0.0f,
			const Vector4f colour = { 1.0f, 1.0f, 1.0f, 1.0f },
			const i32 lifeTime = 1);
		static void DrawCircle2D(
			const Vector2f centrePos,
			const i32 radius = 50,
			const i32 segments = 20,
			const Vector4f colour = { 1.0f, 1.0f, 1.0f, 1.0f },
			i32 lifeTime = 1);
		inline static std::vector<u32> GenerateIndicies(usize& size)
		{
			size = LINE2D_INDEX_COUNT * LINE2D_MAX_LINES;
			std::vector<u32> index_buffer_data(size);

			for (usize i = 0; i < size; i += LINE2D_INDEX_COUNT)
			{
				i32 offset = 4 * (i / LINE2D_INDEX_COUNT);

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
	};
}



#endif // CODEX_RENDERER_DEBUG_DRAW_H
