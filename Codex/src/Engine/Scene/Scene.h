#ifndef CODEX_CORE_SCENE_H
#define CODEX_CORE_SCENE_H

#include <sdafx.h>

#include "../Core/Camera.h"
#include "../Renderer/Renderer.h"
#include "../Scene/SpriteSheet.h"
#include "../Core/ResourceHandler.h"
#include "../Core/MouseHandler.h"
#include "../Core/KeyHandler.h"

namespace codex {
	// Forward declarations
	class Window;
	class Entity;

	class Scene
	{
		friend class Window;
		friend class Entity;

	protected:
		i32 m_Width;
		i32 m_Height;
		std::unique_ptr<Camera> m_Camera;
		Renderer* m_Renderer;
		entt::registry m_Registry;

	private:
		bool m_Running;

	public:
		Scene(Renderer* renderer, const i32 width, const i32 height);
		virtual ~Scene() = default;

	public:
		inline Camera* GetCamera() const
			{ return m_Camera.get(); }
		inline Vector2f GetMousePositionInWorld()
			{ return Vector2f(MouseHandler::GetMouseX() + m_Camera->position.x, MouseHandler::GetMouseY() + m_Camera->position.y); }

	public:
		template<typename T>
		inline std::vector<Entity> GetAllEntitiesWithComponent()
		{
			auto view = m_Registry.view<T>();
			std::vector<Entity> entities;
			entities.reserve(view.size_hint());
			for (auto& e : view)
				entities.emplace_back(e, this);
			return entities;
		}

	public:
		Entity CreateEntity(const std::string_view tag = "default tag");
		void RemoveEntity(const Entity entity);
		void RemoveEntity(const u32 entity);
		std::vector<Entity> GetAllEntitesWithTag(const std::string_view tag);

	public:
		void OnWindowResize_Event(const i32 newWidth, const i32 newHeight)
		{
			m_Camera->SetWidth(newWidth);
			m_Camera->SetHeight(newHeight);
		}

	private:
		virtual void Init();

	public:
		virtual void Start() = 0;
		virtual void Update(const f32 deltaTime) = 0;
		virtual void Render(const f32 deltaTime) = 0;
	};
}

#endif // CODEX_CORE_SCENE_H
