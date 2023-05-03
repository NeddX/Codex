#ifndef CODEX_SCENE_SPRITE_SHEET_H
#define CODEX_SCENE_SPRITE_SHEET_H

#include <sdafx.h>

#include "Sprite.h"
#include "../Core/Geomtryd.h"
#include "../Renderer/Texture2D.h"

namespace Codex
{
	class SpriteSheet
	{
	private:
		std::shared_ptr<Texture2D> m_Texture;
		std::vector<Vector2f> m_SpriteCoords;
		std::vector<Sprite> m_Sprites;
		int m_Width;
		int m_Height;
		int m_TotalSpriteCount;
		int m_SpriteWidth;
		int m_SpriteHeight;

	public:
		SpriteSheet(
			std::shared_ptr<Texture2D> texture,
			int spriteWidth,
			int spriteHeight,
			int spriteCount,
			int space) : 
			m_Texture(texture), 
			m_TotalSpriteCount(spriteCount), 
			m_SpriteWidth(spriteWidth), 
			m_SpriteHeight(spriteHeight),
			m_Width(texture->GetWidth()),
			m_Height(texture->GetHeight())
		{
			int x = space, y = space;
			int tex_width = texture->GetWidth(), tex_height = texture->GetHeight();
			for (int i = 0; i < spriteCount; ++i)
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
		inline std::vector<Sprite> GetSprites() const		{ return m_Sprites; }
		inline Sprite GetSpriteAt(size_t index) const		{ return m_Sprites[index]; }
		inline Sprite GetSpriteAt(int x, int y) const		{ return m_Sprites[(y / m_SpriteWidth) * (m_Width / m_SpriteWidth) + (x / m_SpriteHeight)]; }
		inline int GetSpriteCount() const					{ return m_TotalSpriteCount; }
	};
}

#endif // CODEX_SCENE_SPRITE_SHEET_H