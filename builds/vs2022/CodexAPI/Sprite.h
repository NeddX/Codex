#ifndef CODEX_NATIVE_API_SPRITE_H
#define CODEX_NATIVE_API_SPRITE_H

#include "Codex.h"
#include "Texture2D.h"

namespace
{
	class Sprite
	{
	private:
		Texture2D m_Texture;
		Codex::Rectf m_SrcRect;

	public:
		inline Texture2D GetTexture() { return m_Texture; }
		inline Codex::Rectf GetTextureCoords() { return m_SrcRect; }

	public:
		Sprite(Texture2D texture, Codex::Rectf textureCoords) :
			m_Texture(texture), m_SrcRect(textureCoords)
		{

		}
	};
}

#endif // CODEX_NATIVE_API_SPRITE_H