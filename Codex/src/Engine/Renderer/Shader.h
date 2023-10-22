#ifndef CODEX_RENDERER_SHADER_H
#define CODEX_RENDERER_SHADER_H

#include <sdafx.h>

#include "../Core/IResource.h"
#include "../Core/Exception.h"

namespace codex {
	class ShaderException : public CodexException
	{
		using CodexException::CodexException;

    public:
        inline const char* default_message() const noexcept override { return "Bad shader."; }
    };

	class ShaderNotFoundException : public ShaderException
	{
		using ShaderException::ShaderException;

    public:
        inline const char* default_message() const noexcept override { return "Shader was not found."; }
	};

    class Shader : public IResource
	{
		friend class ResourceHandler;

	private:
		std::unique_ptr<mgl::Shader> m_RawShader;

	public:
		Shader(const char* filePath)
		{
			m_RawShader = std::make_unique<mgl::Shader>(filePath);
		}

	public:
		inline void Bind() const
			{ m_RawShader->Bind(); }
		inline void Unbind() const
			{ m_RawShader->Unbind(); }
		inline void SetUniform1i(const char* name, const i32 value)
			{ m_RawShader->SetUniform1i(name, value); }
		inline void SetUniform1iArr(const char* name, const u32 count, const i32* value)
			{ m_RawShader->SetUniform1iArr(name, count, value); };
		inline void SetUniform1f(const char* name, const f32 value)
			{ m_RawShader->SetUniform1f(name, value); }
		inline void SetUniform2f(const char* name, const f32 a0, const f32 a1)
			{ m_RawShader->SetUniform2f(name, a0, a1); }
		inline void SetUniform4f(const char* name, const f32 a0, const f32 a1, const f32 a2, const f32 a3)
			{ m_RawShader->SetUniform4f(name, a0, a1, a2, a3); }
		inline void SetUniform3f(const char* name, const f32 a0, const f32 a1, const f32 a2)
			{ m_RawShader->SetUniform3f(name, a0, a1, a2); }
		inline void SetUniformMat4f(const char* name, const glm::mat4& matrix)
			{ m_RawShader->SetUniformMat4f(name, matrix); }
		inline void SetUniformMat4f(const char* name, const f32* matrix)
			{ m_RawShader->SetUniformMat4f(name, matrix); }
	};
}

#endif // CODEX_RENDERER_SHADER_H
