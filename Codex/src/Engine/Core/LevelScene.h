#ifndef CODEX_CORE_LEVEL_SCENE_H
#define CODEX_CORE_LEVEL_SCENE_H

#include <sdafx.h>

#include "Scene.h"

namespace Codex {
	class LevelScene : public Scene
	{
	public:
		LevelScene(Renderer* renderer, const int width, const int height);
	
	public:
		void Start() override;
		void Update(const float deltaTime) override;
		void Render(const float deltaTime) override;
	};
}

#endif // CODEX_CORE_LEVEL_SCENE_H
