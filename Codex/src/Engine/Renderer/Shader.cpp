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
		glDeleteProgram(m_ProgramID);
		glDeleteShader(m_VertexShaderID);
		glDeleteShader(m_FragmentShaderID);
		glUseProgram(0);

		delete m_VertexSource;
		delete m_FragmentSource;
	}

	void Shader::Compile()
	{
		// Compile Vertex Shader
		m_VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(m_VertexShaderID, 1, &m_VertexSource, nullptr);
		glCompileShader(m_VertexShaderID);
		int v_compile_status;
		glGetShaderiv(m_VertexShaderID, GL_COMPILE_STATUS, &v_compile_status);
		if (!v_compile_status)
		{
			int length;
			glGetShaderiv(m_VertexShaderID, GL_INFO_LOG_LENGTH, &length);
			char* err_msg = (char*) alloca(length);
			glGetShaderInfoLog(m_VertexShaderID, length, nullptr, err_msg);
			fmt::println("[Error] :: Failed to compile Vertex Shader!\n\tGLMSG > {}", err_msg);
			auto err = std::runtime_error("");
			throw err;
		}

		// Compile fragment shader
		m_FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(m_FragmentShaderID, 1, &m_FragmentSource, nullptr);
		glCompileShader(m_FragmentShaderID);
		int f_compile_status;
		glGetShaderiv(m_FragmentShaderID, GL_COMPILE_STATUS, &f_compile_status);
		if (!f_compile_status)
		{
			int length;
			glGetShaderiv(m_FragmentShaderID, GL_INFO_LOG_LENGTH, &length);
			char* err_msg = (char*) alloca(length);
			glGetShaderInfoLog(m_FragmentShaderID, length, nullptr, err_msg);
			fmt::println("[Error] :: Failed to compile Fragment Shader!\n\tGLMSG > {}", err_msg);
			auto err = std::runtime_error("");
			throw err;
		}

		m_ProgramID = glCreateProgram();
		glAttachShader(m_ProgramID, m_VertexShaderID);
		glAttachShader(m_ProgramID, m_FragmentShaderID);
		glLinkProgram(m_ProgramID); 
		int p_link_status;
		glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &p_link_status);
		if (!p_link_status)
		{
			int length;
			glGetProgramiv(m_ProgramID, GL_INFO_LOG_LENGTH, &length);
			char* err_msg = (char*) alloca(length);
			glGetProgramInfoLog(m_ProgramID, length, nullptr, err_msg);
			fmt::println("[Error] :: Failed to link shaders!\n\tGLMSG > {}", err_msg);
			auto err = std::runtime_error("");
			throw err;
		}
	}

	void Shader::Use()
	{
		glUseProgram(m_ProgramID);
	}

	void Shader::Detach()
	{
		glUseProgram(0); 
	}

	void Shader::UploadMatrix4f(const char* varName, Matrix4f& mat4)
	{
		int var_location = glGetUniformLocation(m_ProgramID, varName);
		float* matrix_buffer = glm::value_ptr(mat4);
		glUniformMatrix4fv(var_location, 1, false, matrix_buffer);
	}

	void Shader::Dispose()
	{
		delete this;
	}
}
*/
