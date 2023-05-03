#ifndef CODEX_SCENE_ECS_SPRITE_RENDERER_H
#define CODEX_SCENE_ECS_SPRITE_RENDERER_H

#include "ECS.h"

#include "../Sprite.h"
#include "../../Renderer/Texture2D.h"

namespace Codex
{
	class SpriteRendererComponent : public IComponent
	{
	private:
		Vector4f m_Colour;
		Sprite m_Sprite;
		TransformComponent* m_Trans;
		int m_ZIndex;

	public:
		SpriteRendererComponent(const Vector4f colour) : 
			m_Colour(colour), m_Sprite(nullptr), m_ZIndex(0)
		{
			
		}
		SpriteRendererComponent(Sprite sprite, Vector4f colour = Vector4f(1.0f, 1.0f, 1.0f, 1.0f)) :
			m_Sprite(sprite), m_Colour(colour), m_ZIndex(0)
		{
			
		}

	public:
		inline Vector4f GetColour() const						{ return m_Colour; }
		inline Sprite GetSprite() const							{ return m_Sprite; }
		inline int GetZIndex() const							{ return m_ZIndex; }
		inline void SetSprite(Sprite newSprite)					{ m_Sprite = newSprite; }
		inline void SetZIndex(int newIndex)						{ m_ZIndex = newIndex; }

	public:
		void Start() override
		{
			m_Trans = &m_Parent->GetComponent<TransformComponent>();
		}
		void Update(float deltaTime) override
		{

		}
		void Render(float deltaTime, SpriteBatchRenderer* renderer) override
		{
			Rect tex_coords = m_Sprite.GetTextureCoords();
			renderer->RenderRect(
				m_Sprite.GetTexture().get(),
				tex_coords,
				{
					m_Trans->position.x,
					m_Trans->position.y,
					tex_coords.w * m_Trans->scale.x,
					tex_coords.h * m_Trans->scale.y,
				},
				m_ZIndex
			);
		}
	};
}

#endif // CODEX_SCENE_ECS_SPRITE_RENDERERE_2D_H