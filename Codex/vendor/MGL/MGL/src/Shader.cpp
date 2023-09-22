#include "Shader.h"
#include "Renderer.h"

#include <iostream>
#include <fstream>
#include <string>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace mgl
{
	Shader::Shader(const char* filePath) : m_FilePath(filePath)
	{
		GL_Call(m_RendererId = glCreateProgram());
		ParseShaderFile();
		CompileShader();
	}

	Shader::~Shader()
	{
		delete[] m_VertexSource;
		delete[] m_FragmentSource;
		GL_Call(glUseProgram(0));
		GL_Call(glDeleteProgram(m_RendererId));
		GL_Call(glDeleteShader(m_VertexShaderId));
		GL_Call(glDeleteShader(m_FragmentShaderId));
	}

	void Shader::Bind() const
	{
		GL_Call(glUseProgram(m_RendererId));
	}

	void Shader::Unbind() const
	{
		GL_Call(glUseProgram(0));
	}
	
	void Shader::SetUniform1i(const char* name, const int32_t value)
	{
		GL_Call(glUniform1i(GetUniformLocation(name), value));
	}

	void Shader::SetUniform1iArr(const char* name, const uint32_t count, const int32_t* value)
	{
		glUniform1iv(GetUniformLocation(name), count, value);
	}
	
	void Shader::SetUniform1f(const char* name, const float value)
	{
		GL_Call(glUniform1f(GetUniformLocation(name), value));
	}
	
	void Shader::SetUniform2f(const char* name, const float a0, const float a1)
	{
		GL_Call(glUniform2f(GetUniformLocation(name), a0, a1));
	}

	void Shader::SetUniform4f(const char* name, const float a0, const float a1, const float a2, const float a3)
	{
		GL_Call(glUniform4f(GetUniformLocation(name), a0, a1, a2, a3));
	}

	void Shader::SetUniform3f(const char* name, const float a0, const float a1, const float a2)
	{
		GL_Call(glUniform3f(GetUniformLocation(name), a0, a1, a2));
	}

	void Shader::SetUniformMat4f(const char* name, const glm::mat4& matrix)
	{
		GL_Call(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix)));
	}

	void Shader::SetUniformMat4f(const char* name, const float* matrix)
	{
		GL_Call(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, matrix));
	}

	void Shader::ParseShaderFile()
	{
		std::ifstream fs(m_FilePath);

		if (!fs.is_open())
		{
			std::cerr << "[OpenGL]::[ERROR] >> Failed to open file '" << m_FilePath << "'!" << std::endl;
			throw std::runtime_error("[OpenGL]::[ERROR] >> Failed to open file shader file!");
		}

		std::string line;
		std::string vertex_code;
		std::string fragment_code;
		bool vertex = false, fragment = false;
		while (std::getline(fs, line))
		{
			if (line == "#shader_type vertex")
			{
				vertex = true;
				fragment = false;
			}
			else if (line == "#shader_type fragment")
			{
				fragment = true;
				vertex = false;
			}
			else if (vertex)
				vertex_code.append(line + '\n');
			else if (fragment)
				fragment_code.append(line + '\n');
		}
		fs.close();

		m_VertexSource = new char[vertex_code.size() + 1];
		m_FragmentSource = new char[fragment_code.size() + 1];
		std::memset(m_VertexSource, 0, vertex_code.size() + 1);
		std::memset(m_FragmentSource, 0, fragment_code.size() + 1);
		std::memcpy(m_VertexSource, vertex_code.c_str(), vertex_code.size());
		std::memcpy(m_FragmentSource, fragment_code.c_str(), fragment_code.size());
	}

	bool Shader::CompileShader()
	{
		GL_Call(m_VertexShaderId = glCreateShader(GL_VERTEX_SHADER));
		GL_Call(m_FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER));

		GL_Call(glShaderSource(m_VertexShaderId, 1, &m_VertexSource, nullptr));
		GL_Call(glShaderSource(m_FragmentShaderId, 1, &m_FragmentSource, nullptr));

		GL_Call(glCompileShader(m_VertexShaderId));
		int v_compile_status;
		glGetShaderiv(m_VertexShaderId, GL_COMPILE_STATUS, &v_compile_status);
		if (!v_compile_status)
		{
			int length;
			glGetShaderiv(m_VertexShaderId, GL_INFO_LOG_LENGTH, &length);
			char* err_msg = (char*)alloca(length);
			glGetShaderInfoLog(m_VertexShaderId, length, nullptr, err_msg);
			std::cout << "[OpenGL]::[ERROR] >> Failed to compile Vertex Shader!\n\tGLMSG > " << err_msg << std::endl;
		}
		GL_Call(glCompileShader(m_FragmentShaderId));
		int f_compile_status;
		glGetShaderiv(m_FragmentShaderId, GL_COMPILE_STATUS, &f_compile_status);
		if (!f_compile_status)
		{
			int length;
			glGetShaderiv(m_FragmentShaderId, GL_INFO_LOG_LENGTH, &length);
			char* err_msg = (char*)alloca(length);
			glGetShaderInfoLog(m_FragmentShaderId, length, nullptr, err_msg);
			std::cout << "[OpenGL]::[ERROR] >> Failed to compile Fragment Shader!\n\tGLMSG > " << err_msg << std::endl;
		}
		
		GL_Call(glAttachShader(m_RendererId, m_VertexShaderId));
		GL_Call(glAttachShader(m_RendererId, m_FragmentShaderId));
		
		GL_Call(glLinkProgram(m_RendererId));
		int p_link_status;
		glGetProgramiv(m_RendererId, GL_LINK_STATUS, &p_link_status);
		if (!p_link_status)
		{
			int length;
			glGetProgramiv(m_RendererId, GL_INFO_LOG_LENGTH, &length);
			char* err_msg = (char*)alloca(length);
			glGetProgramInfoLog(m_RendererId, length, nullptr, err_msg);
			std::cout << "[OpenGL]::[ERROR] >> Failed to link shaders!\n\tGLMSG > " << err_msg << std::endl;
		}
		return true;
	}

	int32_t Shader::GetUniformLocation(const char* name)
	{
		int32_t location;
		const auto& found = m_UniformLocations.find(name);
		if (found != m_UniformLocations.end())
			location = found->second;
		else
		{
			GL_Call(location = glGetUniformLocation(m_RendererId, name));
			m_UniformLocations[name] = location;
			if (location == -1)
				std::cout << "[OpenGL]::[WARNING] >> Uniform '" << name << "' does not exist!" << std::endl;
		}
		return location;
	}
}
