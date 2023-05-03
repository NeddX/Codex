#ifndef CODEX_CORE_EDITOR_SCENE_H
#define CODEX_CORE_EDITOR_SCENE_H

#include <sdafx.h>

#include "Scene.h"
#include "../Editor/EditorLayer.h"

namespace Codex
{
	class EditorScene : public Scene
	{
	public:
		void* operator new(size_t size)
		{
			void* ptr = std::malloc(size);
			fmt::println("[Memory] :: Allocated memory.\n\tFile: {}\n\tLine: {}\n\tSize: {}\n\tAddress: {}",
				__FILE__, __LINE__, size, ptr);
			return ptr;
		}
		void operator delete(void* ptr)
		{
			fmt::println("[Memory] :: Deallocated memory.\n\tFile: {}\n\tLine: {}\n\tAddress: {}", __FILE__, __LINE__, ptr);
			std::free(ptr);
		}

	private:
		SpriteBatchRenderer* m_SpriteBatch;
		std::shared_ptr<Shader> m_Shader;

	public:
		EditorScene(Renderer* renderer, int width, int height);
		~EditorScene();

	public:
		void Start() override;
		void Update(float deltaTime) override;
	};
}

#endif // CODEX_CORE_EDITOR_SCENE_H
