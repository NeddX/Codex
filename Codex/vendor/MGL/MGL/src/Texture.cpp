#include "Texture.h"
#include "Renderer.h"

#include <stb_image.h>
#include <stdexcept>

namespace mgl
{
	Texture::Texture(const char* filePath, const TextureProperties properties) : m_FilePath(filePath)
	{
		m_RendererID = 0;
		m_Buffer = nullptr;
		m_Width = 0;
		m_Height = 0;
		m_GPP = 0;
		GL_Call(glGenTextures(1, &m_RendererID));
		GL_Call(glBindTexture(GL_TEXTURE_2D, m_RendererID));
		
		stbi_set_flip_vertically_on_load(1);
		m_Buffer = stbi_load(filePath, &m_Width, &m_Height, &m_GPP, 4);
		if (!m_Buffer)
		{
			throw std::runtime_error("[OpenGL]::[ERROR] >> Could not open texture file for reading!");
		}

		switch (properties.textureWrapMode)
		{
			case TextureWrapMode::MIRROR:
				GL_Call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT));
				GL_Call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT));
				break;
			case TextureWrapMode::STRETCH:
				GL_Call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
				GL_Call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
				break;
			case TextureWrapMode::BORDER:
				GL_Call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
				GL_Call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
				break;
		}

		switch (properties.textureFilterMode)
		{
			case TextureFilterMode::BLUR:
				GL_Call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
				GL_Call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
				GL_Call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
				break;
			case TextureFilterMode::PIXELATE:
				GL_Call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
				GL_Call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
				GL_Call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
				break;
		}
		
		GL_Call(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Buffer));
		Bind();
	}
	
	Texture::~Texture()
	{
		Unbind();
		if (m_Buffer) stbi_image_free(m_Buffer);
		GL_Call(glDeleteTextures(1, &m_RendererID));
	}
	
	void Texture::Bind(uint32_t slot)
	{
		m_Slot = GL_TEXTURE0 + slot;
		GL_Call(glActiveTexture(m_Slot));
		GL_Call(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	}
	
	void Texture::Unbind() const
	{
		GL_Call(glBindTexture(GL_TEXTURE_2D, 0));
	}
}