#ifndef CODEX_SCENE_SPRITE_SHEET_H
#define CODEX_SCENE_SPRITE_SHEET_H

#include <sdafx.h>

#include "Sprite.h"
#include "../Core/Geomtryd.h"
#include "../Renderer/Texture2D.h"

namespace Codex {
	class SpriteSheet
	{
	private:
		std::shared_ptr<Texture2D> m_Texture;
		std::vector<Vector2f> m_SpriteCoords;
		std::vector<Sprite> m_Sprites;
		i32 m_Width;
		i32 m_Height;
		i32 m_TotalSpriteCount;
		i32 m_SpriteWidth;
		i32 m_SpriteHeight;

	public:
		SpriteSheet(
			std::shared_ptr<Texture2D> texture,
			i32 spriteWidth,
			i32 spriteHeight,
			i32 spriteCount,
			i32 space) : 
			m_Texture(texture), 
			m_Width(texture->GetWidth()),
			m_Height(texture->GetHeight()),
			m_TotalSpriteCount(spriteCount),
			m_SpriteWidth(spriteWidth),
			m_SpriteHeight(spriteHeight)
		{
			i32 x = space, y = space;
			i32 tex_width = texture->GetWidth();
			for (i32 i = 0; i < spriteCount; ++i)
			{
				if (x >= tex_width + space)
				{
					y += spriteHeight + space;
					x = space;
				}
				m_SpriteCoords.emplace_back(x, y);
				m_Sprites.emplace_back(texture, Rectf(x, y, spriteWidth, spriteHeight));
				x += spriteWidth + space;
			}
		}

	public:
		inline std::vector<Sprite> GetSprites() const
			{ return m_Sprites; }
		inline Sprite GetSpriteAt(const usize index) const
			{ return m_Sprites[index]; }
		inline Sprite GetSpriteAt(const i32 x, const i32 y) const
			{ return m_Sprites[(y / m_SpriteWidth) * (m_Width / m_SpriteWidth) + (x / m_SpriteHeight)]; }
		inline i32 GetSpriteCount() const
			{ return m_TotalSpriteCount; }
	};
}

#endif // CODEX_SCENE_SPRITE_SHEET_H
