#ifndef CODEX_SCENE_COMPONENTS_H
#define CODEX_SCENE_COMPONENTS_H

#include <sdafx.h>

#include "../Sprite.h"
//#include "TransformComponent.h"
//#include "SpriteRendererComponent.h"

#include "../../Core/Geomtryd.h"

namespace Codex
{
	struct TagComponent
	{
	public:
		const char* tag;

	public:
		TagComponent()
		{
			tag = "default tag";
		}
		TagComponent(const char* tag) : tag(tag)
		{

		}
	};

	struct TransformComponent
	{
	public:
		Vector2f position;
		Vector2f scale;

	public:
		TransformComponent(Vector2f position = Vector2f(), Vector2f scale = Vector2f(1.0f, 1.0f)) :
			position(position), scale(scale)
		{

		}
	};

	/*struct TransformComponent
	{
	public:
		Matrix4f transform;

	public:
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(Matrix4f transform) : transform(transform) {}
	};*/

	struct SpriteRendererComponent
	{
	private:
		Vector4f m_Colour;
		Sprite m_Sprite;
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
		SpriteRendererComponent(const SpriteRendererComponent&) = default;

	public:
		inline Vector4f GetColour() const { return m_Colour; }
		inline Sprite& GetSprite() { return m_Sprite; }
		inline int GetZIndex() const { return m_ZIndex; }
		inline void SetSprite(Sprite newSprite) { m_Sprite = newSprite; }
		inline void SetZIndex(int newIndex) { m_ZIndex = newIndex; }
	};

	struct DummyComponent
	{
		const char* dummy = "i am a dummy.";
	public:
		DummyComponent() = default;
	};
}

#endif // CODEX_SCENE_COMPONENTS_H