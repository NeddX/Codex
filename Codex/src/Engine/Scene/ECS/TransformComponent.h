#ifndef CODEX_SCENE_ECS_TRANSFORM_COMPONENT_H
#define CODEX_SCENE_ECS_TRANSFORM_COMPONENT_H

#include "ECS.h"

namespace Codex {
	class TransformComponent : public IComponent
	{
	public:
		Vector2f position;
		Vector2f scale;

	public:
		TransformComponent(Vector2f position = Vector2f(), Vector2f scale = Vector2f(1.0f, 1.0f)) :
			position(position), scale(scale)
		{

		}

	public:
		void Start() override
		{

		}
		void Update(float deltaTime) override
		{

		}
		void Render(float deltaTime, SpriteBatchRenderer* renderer) override
		{

		}
	};
}

#endif // CODEX_SCENE_ECS_TRANSFORM_COMPONENT_H