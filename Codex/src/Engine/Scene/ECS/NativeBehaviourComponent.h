#ifndef CODEX_SCENE_ECS_NATIVE_BEHAVIOUR_COMPONENT_H
#define CODEX_SCENE_ECS_NATIVE_BEHAVIOUR_COMPONENT_H

#include "ECS.h"

namespace Codex
{
	class NativeBehaviourComponent : public IComponent
	{
	public:
		NativeBehaviourComponent() = default;

	public:
		void Start() override
		{
			fmt::println("NBC started!");
		}
		void Update(float deltaTime) override
		{

		}
		void Render(float deltaTime, SpriteBatchRenderer* renderer)
		{

		}
	};
}

#endif // CODEX_SCENE_ECS_NATIVE_BEHAVIOUR_COMPONENT_H