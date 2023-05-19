#ifndef CODEX_CORE_EDITOR_SCENE_H
#define CODEX_CORE_EDITOR_SCENE_H

#include <sdafx.h>

#include "Scene.h"
#include "../Editor/EditorLayer.h"

namespace Codex {
	enum class EditorAction : uint8_t 
	{
		Select,
		TilemapBrush,
		TilemapErase
	};

	class EditorScene : public Scene
	{
#ifdef CDX_DEBUG_CUSTOM_ALLOCATORS
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
#endif

	private:
		EntityManager m_EditorSceneManager;
		std::shared_ptr<SpriteBatchRenderer> m_SpriteBatch;
		std::shared_ptr<Shader> m_Shader;
		Vector2f m_SelectedTileCoord;
		EditorAction m_CurrentActiveAction;

	public:
		inline Vector2f GetSelectedTileCoord() const { return m_SelectedTileCoord; }
		inline EditorAction GetActiveAction() const { return m_CurrentActiveAction; }
		inline void SetSelectedTileCoord(Vector2f newCoord) { m_SelectedTileCoord = newCoord; }
		inline void SetActiveAction(EditorAction newAction) { m_CurrentActiveAction = newAction; }

	public:
		EditorScene(Renderer* renderer, int width, int height);
		~EditorScene();

	public:
		void Start() override;
		void Update(float deltaTime) override;
	};
}

#endif // CODEX_CORE_EDITOR_SCENE_H
