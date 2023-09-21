/*#include "include/Shader.h"

namespace Codex
{
	Shader::Shader(const char* filePath)
	{
		m_FilePath = filePath;
		std::ifstream fs(filePath);
		
		if (!fs.is_open())
		{
			auto err = std::runtime_error(fmt::format("Couldn't open shader file '{}'.", filePath));
			throw err;
		}
		
		std::string line;
		std::string* vertex_code = new std::string();
		std::string* fragment_code = new std::string();
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
				vertex_code->append(line + '\n');
			else if (fragment)
				fragment_code->append(line + '\n');
		}
		fs.close();
		
		m_VertexSource = vertex_code->c_str();
		m_FragmentSource = fragment_code->c_str();
	}

	Shader::~Shader()
	{
		glUseProgram(0);
		glDeleteProgram(m_ProgramId);
		glDeleteShader(m_VertexShaderId);
		glDeleteShader(m_FragmentShaderId);
		glUseProgram(0);

		delete m_VertexSource;
		delete m_FragmentSource;
	}

	void Shader::Compile()
	{
		// Compile Vertex Shader
		m_VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(m_VertexShaderId, 1, &m_VertexSource, nullptr);
		glCompileShader(m_VertexShaderId);
		int v_compile_status;
		glGetShaderiv(m_VertexShaderId, GL_COMPILE_STATUS, &v_compile_status);
		if (!v_compile_status)
		{
			int length;
			glGetShaderiv(m_VertexShaderId, GL_INFO_LOG_LENGTH, &length);
			char* err_msg = (char*) alloca(length);
			glGetShaderInfoLog(m_VertexShaderId, length, nullptr, err_msg);
			fmt::println("[Error] :: Failed to compile Vertex Shader!\n\tGLMSG > {}", err_msg);
			auto err = std::runtime_error("");
			throw err;
		}

		// Compile fragment shader
		m_FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(m_FragmentShaderId, 1, &m_FragmentSource, nullptr);
		glCompileShader(m_FragmentShaderId);
		int f_compile_status;
		glGetShaderiv(m_FragmentShaderId, GL_COMPILE_STATUS, &f_compile_status);
		if (!f_compile_status)
		{
			int length;
			glGetShaderiv(m_FragmentShaderId, GL_INFO_LOG_LENGTH, &length);
			char* err_msg = (char*) alloca(length);
			glGetShaderInfoLog(m_FragmentShaderId, length, nullptr, err_msg);
			fmt::println("[Error] :: Failed to compile Fragment Shader!\n\tGLMSG > {}", err_msg);
			auto err = std::runtime_error("");
			throw err;
		}

		m_ProgramId = glCreateProgram();
		glAttachShader(m_ProgramId, m_VertexShaderId);
		glAttachShader(m_ProgramId, m_FragmentShaderId);
		glLinkProgram(m_ProgramId); 
		int p_link_status;
		glGetProgramiv(m_ProgramId, GL_LINK_STATUS, &p_link_status);
		if (!p_link_status)
		{
			int length;
			glGetProgramiv(m_ProgramId, GL_INFO_LOG_LENGTH, &length);
			char* err_msg = (char*) alloca(length);
			glGetProgramInfoLog(m_ProgramId, length, nullptr, err_msg);
			fmt::println("[Error] :: Failed to link shaders!\n\tGLMSG > {}", err_msg);
			auto err = std::runtime_error("");
			throw err;
		}
	}

	void Shader::Use()
	{
		glUseProgram(m_ProgramId);
	}

	void Shader::Detach()
	{
		glUseProgram(0); 
	}

	void Shader::UploadMatrix4f(const char* varName, Matrix4f& mat4)
	{
		int var_location = glGetUniformLocation(m_ProgramId, varName);
		float* matrix_buffer = glm::value_ptr(mat4);
		glUniformMatrix4fv(var_location, 1, false, matrix_buffer);
	}

	void Shader::Dispose()
	{
		delete this;
	}
}
*/
