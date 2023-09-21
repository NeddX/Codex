#include "DebugDraw.h"
#include "../Core/Window.h"
#include "../Core/Geomtryd.h"

namespace Codex {
	DebugDraw* DebugDraw::m_Instance = nullptr;
	std::vector<Line2D> DebugDraw::m_Lines;
	std::array<float, LINE2D_MAX_LINES * LINE2D_VERTEX_SIZE> DebugDraw::m_Verticies;

	void DebugDraw::Init()
	{
		if (!m_Instance)
		{
			m_Instance = new DebugDraw();

			// TODO: Set line width

			m_Instance->m_Shader = std::make_unique<Shader>("debugLine2D.glsl");

			m_Instance->m_Vao = std::make_unique<mgl::VertexArray>();
			m_Instance->m_Vao->Bind();

			m_Instance->m_Vbo = std::make_unique<mgl::VertexBuffer>();
			m_Instance->m_Vbo->Bind();
			m_Instance->m_Vbo->SetBuffer<float>(nullptr, LINE2D_VERTEX_SIZE * sizeof(float) * LINE2D_MAX_LINES, mgl::BufferUsage::DYNAMIC_DRAW); // you basically allocate space and then upload the data

			/*
			m_Ebo = std::make_unique<mgl::IndexBuffer>();
			m_Ebo->Bind();
			size_t size = 0;
			auto index_buffer_data = GenerateIndicies(size);
			m_Ebo->SetBuffer(index_buffer_data.data(), size);
			*/

			m_Instance->m_Layout = std::make_unique<mgl::VertexBufferLayout>();
			m_Instance->m_Layout->Push<float>(3);
			m_Instance->m_Layout->Push<float>(4);

			m_Instance->m_Vao->AddBuffer(m_Instance->m_Vbo.get(), m_Instance->m_Layout.get());

			std::fill(m_Verticies.begin(), m_Verticies.end(), 0.0f);
			fmt::println("DebugDraw subsystem initialized.");
		}
	}

	void DebugDraw::Begin()
	{
		if (!m_Instance) return;

		for (int i = 0; i < m_Lines.size(); ++i)
		{
			if (m_Lines[i].BeginFrame() < 0)
				m_Lines.erase(m_Lines.begin() + i--);
		}
	}

	void DebugDraw::Render()
	{
		if (!m_Instance) return;

		if (m_Lines.size() <= 0) return;

		int index = 0;
		for (const auto& line : m_Lines)
		{
			for (int i = 0; i < 2; ++i)
			{
				Vector3f pos = (i == 0) 
					? Vector3f(line.GetSource(), 0.0f) 
					: Vector3f(line.GetDestination(), 0.0f);
				Vector4f colour = line.GetColour();

				// TODO: Please for the love of God, write your own vector class that is compatible with glm.
				//std::memcpy(m_Verticies.data() + index, ValuePtr(pos), sizeof(3 * sizeof(float)));
				//std::memcpy(m_Verticies.data() + index + sizeof(3 * sizeof(float)), ValuePtr(colour), sizeof(4 * sizeof(float)));

				m_Verticies[index] = pos.x;
				m_Verticies[index + 1] = pos.y;
				m_Verticies[index + 2] = pos.z;
				m_Verticies[index + 3] = colour.x;
				m_Verticies[index + 4] = colour.y;
				m_Verticies[index + 5] = colour.z;
				m_Verticies[index + 6] = colour.w;

				index += 7;
			}

			// draw here
			m_Instance->m_Shader->Bind();
			m_Instance->m_Vao->Bind();
			//m_Ebo->Bind();
			m_Instance->m_Vbo->Bind();
			m_Instance->m_Vbo->SetBufferSubData<float>(m_Verticies.data(), 0, LINE2D_VERTEX_SIZE * m_Lines.size() * sizeof(float));

			auto* camera = Window::Get()->GetCurrentScene()->GetCamera();
			m_Instance->m_Shader->SetUniformMat4f("u_Proj", camera->GetProjectionMatrix());
			m_Instance->m_Shader->SetUniformMat4f("u_View", camera->GetViewMatrix());

			GL_Call(glDrawArrays(GL_LINES, 0, m_Lines.size() * 2));

			m_Instance->m_Shader->Unbind();
			m_Instance->m_Vao->Unbind();
			m_Instance->m_Vbo->Unbind();
			//m_Ebo->Unbind();
		}
	}

	void DebugDraw::Destroy()
	{
		if (m_Instance)
		{
			m_Lines.clear();
			delete m_Instance;
			m_Instance = nullptr;
			fmt::println("DebugDraw subsystem disposed.");
		}
	}

	void DebugDraw::DrawLine2D(Vector2f source, Vector2f destination, Vector4f colour, int lifeTime)
	{
		if (m_Lines.size() < LINE2D_MAX_LINES)
			m_Lines.emplace_back(source, destination, colour, lifeTime);
	}

	void DebugDraw::DrawRect2D(Rectf rect, float angle, Vector4f colour, int lifeTime)
	{
		if (m_Lines.size() + 4 < LINE2D_MAX_LINES)
		{
			glm::vec2 max = glm::vec2 { rect.x, rect.y } - glm::vec2 { rect.w, rect.h } / 2.0f;
			glm::vec2 min = glm::vec2 { rect.x, rect.y } + glm::vec2 { rect.w, rect.h } / 2.0f;

			// vector of vectors, HAHAHAHHHHAHAHAAAHAHAHAAHAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
			std::vector<glm::vec2> lines
			{
				{ min.x, min.y, },
				{ min.x, max.y, },
				{ max.x, max.y, },
				{ max.x, min.y, }
			};

			if (angle != 0.0f)
			{
				for (auto& l : lines)
				{
					auto relative_pos = l - glm::vec2(rect.x, rect.y);
					auto rotated = glm::rotate(relative_pos, angle);
					l = glm::vec2(rect.x, rect.y) + rotated;
				}
			}

			m_Lines.emplace_back(glm::vec2 { lines[0].x, lines[0].y }, glm::vec2 { lines[1].x, lines[1].y }, colour, lifeTime);
			m_Lines.emplace_back(glm::vec2 { lines[0].x, lines[0].y }, glm::vec2 { lines[3].x, lines[3].y }, colour, lifeTime);
			m_Lines.emplace_back(glm::vec2 { lines[1].x, lines[1].y }, glm::vec2 { lines[2].x, lines[2].y }, colour, lifeTime);
			m_Lines.emplace_back(glm::vec2 { lines[2].x, lines[2].y }, glm::vec2 { lines[3].x, lines[3].y }, colour, lifeTime);
		}
	}

	void DebugDraw::DrawCircle2D(Vector2f centrePos, int radius, int segments, Vector4f colour, int lifeTime)
	{
		float angle = 360.0f / segments;
		auto starting_pos = glm::vec2{ centrePos.x + radius, centrePos.y };
		auto relative_pos = starting_pos - centrePos;
		for (int i = 0; i < segments; ++i)
		{
			auto point_one = centrePos + glm::rotate(relative_pos, CX_TO_RADF(angle * (i + 1)));
			auto point_two = centrePos + glm::rotate(relative_pos, CX_TO_RADF(angle * (i + 2)));
			m_Lines.emplace_back(point_one, point_two, colour, lifeTime);
		}
	}
}