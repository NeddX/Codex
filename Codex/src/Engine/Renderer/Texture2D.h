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
		Texture2D(const char* filePath, const TextureProperties textureProperties = {})
		{
			m_RawTexture = std::make_unique<mgl::Texture>(filePath, textureProperties);
		}

	public:
		inline u32 GetSlot() const
			{ return m_RawTexture->GetSlot(); }
		inline i32 GetWidth() const
			{ return m_RawTexture->GetWidth(); }
		inline i32 GetHeight() const
			{ return m_RawTexture->GetHeight(); }
		inline const char* GetFilePath() const
			{ return m_RawTexture->GetFilePath(); }
		inline void Bind(u32 slot = 0)
			{ m_RawTexture->Bind(slot); }
		inline void Unbind() const
			{ m_RawTexture->Unbind(); }
	};
}

#endif // CODEX_CORE_TEXTURE_2D_H
