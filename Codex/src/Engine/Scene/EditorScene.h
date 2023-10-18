#ifndef CODEX_CORE_EDITOR_SCENE_H
#define CODEX_CORE_EDITOR_SCENE_H

#include <sdafx.h>

#include "Scene.h"

namespace codex {
	enum class EditorAction : u8 
	{
		Select,
		TilemapBrush,
		TilemapErase
	};

	class EditorScene : public Scene
	{
	private:
		SpriteBatchRenderer* m_SpriteBatch;
		std::shared_ptr<Shader> m_Shader;
		Vector2f m_SelectedTileCoord;
		EditorAction m_CurrentActiveAction;
		u32 m_SelectedEntityId;

	public:
		inline Vector2f GetSelectedTileCoord() const
			{ return m_SelectedTileCoord; }
		inline EditorAction GetActiveAction() const
			{ return m_CurrentActiveAction; }
		inline void SetSelectedTileCoord(Vector2f newCoord)
			{ m_SelectedTileCoord = newCoord; }
		inline void SetActiveAction(EditorAction newAction)
			{ m_CurrentActiveAction = newAction; }
		inline void SetSelectedEntityId(u32 id)
			{ m_SelectedEntityId = id; }

	public:
		EditorScene(Renderer* renderer, const i32 width, const i32 height);
		~EditorScene();

	public:
		void Start() override;
		void Update(const f32 deltaTime) override;
		void Render(const f32 deltaTime) override;
	};
}

#endif // CODEX_CORE_EDITOR_SCENE_H
