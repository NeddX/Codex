#include "DebugDraw.h"
#include "../Core/Window.h"
#include "../Core/Geomtryd.h"

namespace Codex {
	std::vector<Line2D> DebugDraw::m_Lines;
	std::unique_ptr<mgl::VertexArray> DebugDraw::m_Vao = nullptr;
	std::unique_ptr<mgl::VertexBuffer> DebugDraw::m_Vbo = nullptr;
	std::unique_ptr<mgl::IndexBuffer> DebugDraw::m_Ebo = nullptr;
	std::unique_ptr<mgl::VertexBufferLayout> DebugDraw::m_Layout = nullptr;
	std::unique_ptr<Shader> DebugDraw::m_Shader = nullptr;
	std::array<float, LINE2D_MAX_LINES * LINE2D_VERTEX_SIZE> DebugDraw::m_Verticies;
	bool DebugDraw::m_Initialized = false;

	void DebugDraw::Init()
	{
		m_Initialized = true;

		// TODO: Set line width

		m_Shader = std::make_unique<Shader>("debugLine2D.glsl");

		m_Vao = std::make_unique<mgl::VertexArray>();
		m_Vao->Bind();

		m_Vbo = std::make_unique<mgl::VertexBuffer>();
		m_Vbo->Bind();
		m_Vbo->SetBuffer<float>(nullptr, LINE2D_VERTEX_SIZE * sizeof(float) * LINE2D_MAX_LINES, mgl::BufferUsage::DYNAMIC_DRAW); // you basically allocate space and then upload the data

		/*
		m_Ebo = std::make_unique<mgl::IndexBuffer>();
		m_Ebo->Bind();
		size_t size = 0;
		auto index_buffer_data = GenerateIndicies(size);
		m_Ebo->SetBuffer(index_buffer_data.data(), size);
		*/

		m_Layout = std::make_unique<mgl::VertexBufferLayout>();
		m_Layout->Push<float>(3);
		m_Layout->Push<float>(4);

		m_Vao->AddBuffer(m_Vbo.get(), m_Layout.get());

		std::fill(m_Verticies.begin(), m_Verticies.end(), 0);
	}

	void DebugDraw::Begin()
	{
		if (!m_Initialized)
		{
			Init();
			m_Initialized = true;
		}

		for (int i = 0; i < m_Lines.size(); ++i)
		{
			if (m_Lines[i].BeginFrame() < 0)
				m_Lines.erase(m_Lines.begin() + i--);
		}
	}

	void DebugDraw::Render()
	{
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
			m_Shader->Bind();
			m_Vao->Bind();
			//m_Ebo->Bind();
			m_Vbo->Bind();
			m_Vbo->SetBufferSubData<float>(m_Verticies.data(), 0, LINE2D_VERTEX_SIZE * m_Lines.size() * sizeof(float));

			auto* camera = Window::Get()->GetCurrentScene()->GetCamera();
			m_Shader->SetUniformMat4f("u_Proj", camera->GetProjectionMatrix());
			m_Shader->SetUniformMat4f("u_View", camera->GetViewMatrix());

			GL_Call(glDrawArrays(GL_LINES, 0, m_Lines.size() * 2));

			m_Shader->Unbind();
			m_Vao->Unbind();
			m_Vbo->Unbind();
			//m_Ebo->Unbind();
		}
	}

	void DebugDraw::AddLine2D(Vector2f source, Vector2f destination, Vector4f colour, int lifeTime)
	{
		if (m_Lines.size() < LINE2D_MAX_LINES)
			m_Lines.emplace_back(source, destination, colour, lifeTime);
	}

}