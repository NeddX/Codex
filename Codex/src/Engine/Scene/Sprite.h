#ifndef CODEX_SCENE_SPRITE_H
#define CODEX_SCENE_SPRITE_H

#include "../Core/Geomtryd.h"
#include "../Renderer/Texture2D.h"

namespace Codex {
	class Sprite
	{
	private:
		std::shared_ptr<Texture2D> m_Texture;
		Rectf m_SrcRect;

	public:
		Sprite(std::shared_ptr<Texture2D> texture) :
			m_Texture(texture)
		{
			if (m_Texture) 
				m_SrcRect = { 0.0f, 0.0f, (f32)(texture->GetWidth()), (f32)(texture->GetHeight()) };
		}
		Sprite(std::shared_ptr<Texture2D> texture, const Rectf textureCoords) :
			m_Texture(texture), m_SrcRect(textureCoords)
		{

		}

	public:
		inline std::shared_ptr<Texture2D> GetTexture() const
			{ return m_Texture; }
		inline Rectf GetTextureCoords() const
			{ return m_SrcRect; }
		inline void SetTexture(std::shared_ptr<Texture2D> texture)
			{ m_Texture = texture; }
		inline void SetTextureCoords(const Rectf textureCoords)
			{ m_SrcRect = textureCoords; }
	};
}

#endif // CODEX_SCENE_SPRITE_H
