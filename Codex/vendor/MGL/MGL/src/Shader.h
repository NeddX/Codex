#ifndef MGL_SHADER_H
#define MGL_SHADER_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace mgl
{
	class Shader
	{
	private:
		uint32_t m_RendererID;
		const char* m_FilePath;
		char* m_VertexSource;
		char* m_FragmentSource;
		uint32_t m_VertexShaderID;
		uint32_t m_FragmentShaderID;
		std::unordered_map<std::string, int32_t> m_UniformLocations;

	public:
		Shader(const char* filePath);
		~Shader();

	public:
		void Bind() const;
		void Unbind() const;
		void SetUniform1i(const char* name, const int32_t value);
		void SetUniform1iArr(const char* name, const uint32_t count, const int32_t* value);
		void SetUniform1f(const char* name, const float value);
		void SetUniform2f(const char* name, const float a0, const float a1);
		void SetUniform4f(const char* name, const float a0, const float a1, const float a2, const float a3);
		void SetUniform3f(const char* name, const float a0, const float a1, const float a2);
		void SetUniformMat4f(const char* name, const glm::mat4& matrix);
		void SetUniformMat4f(const char* name, const float* matrix);

	private:
		void ParseShaderFile();
		bool CompileShader();
		int32_t GetUniformLocation(const char* name);
	};
}

#endif // MGL_SHADER_H