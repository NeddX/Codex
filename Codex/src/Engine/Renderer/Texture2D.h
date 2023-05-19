#ifndef CODEX_CORE_TEXTURE_2D_H
#define CODEX_CORE_TEXTURE_2D_H

#include <sdafx.h>

#include "../Core/IResource.h"

namespace Codex {
	typedef mgl::TextureProperties TextureProperties;
	typedef mgl::TextureFilterMode TextureFilterMode;
	typedef mgl::TextureWrapMode TextureWrapMode;

	class Texture2D : public IResource
	{
		friend class ResourceHandler;

	private:
		std::unique_ptr<mgl::Texture> m_RawTexture;

	public:
		Texture2D(const char* filePath, TextureProperties textureProperties = {})
		{
			m_RawTexture = std::make_unique<mgl::Texture>(filePath, textureProperties);
		}

	public:
		inline uint32_t GetSlot() const			{ return m_RawTexture->GetSlot(); }
		inline int GetWidth() const				{ return m_RawTexture->GetWidth(); }
		inline int GetHeight() const			{ return m_RawTexture->GetHeight(); }
		inline const char* GetFilePath() const	{ return m_RawTexture->GetFilePath(); }
		inline void Bind(uint32_t slot = 0)		{ m_RawTexture->Bind(slot); }
		inline void Unbind() const				{ m_RawTexture->Unbind(); }
	};
}

#endif // CODEX_CORE_TEXTURE_2D_H