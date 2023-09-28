#ifndef CODEX_CORE_SCENE_H
#define CODEX_CORE_SCENE_H

#include <sdafx.h>

#include "Camera.h"
#include "../Scene/ECS/ECS.h"
#include "../Renderer/Renderer.h"
#include "../Scene/SpriteSheet.h"
#include "../Core/ResourceHandler.h"
#include "../Core/MouseHandler.h"
#include "../Core/KeyHandler.h"

namespace Codex {
	// Forward declarations
	class Window;

	class Scene
	{
		friend class Window;

	protected:
		int m_Width;
		int m_Height;
		std::unique_ptr<Camera> m_Camera;
		Renderer* m_Renderer;
		EntityManager m_Manager;
		
	private:
		bool m_Running;

	public:
		Scene(Renderer* renderer, int width, int height);
		virtual ~Scene() = default;

	public:
		inline Camera* GetCamera() const
			{ return m_Camera.get(); }
		inline Entity CreateEntity(const std::string& tag = "default tag") 
		{ 
			std::cout << "created entity with tag: " << tag << std::endl;
			auto entity = m_Manager.CreateEntity(tag); 
			return entity;
		}	
		inline Vector2f GetMousePositionInWorld()
			{ return Vector2f(MouseHandler::GetMouseX() + m_Camera->position.x, MouseHandler::GetMouseY() + m_Camera->position.y); }
		inline EntityManager* GetManager()
			{ return &m_Manager; }
		inline void RemoveEntity(const Entity entity)
			{ m_Manager.RemoveEntity(entity); }
		inline void RemoveEntity(const uint32_t entity)
			{ m_Manager.RemoveEntity(Entity((entt::entity)(entity), &m_Manager)); }

	public:
		void OnWindowResize_Event(const int newWidth, const int newHeight)
		{
			m_Camera->SetWidth(newWidth);
			m_Camera->SetHeight(newHeight);
		}

	private:
		virtual void Init();

	public:
		virtual void Start() = 0;
		virtual void Update(const float deltaTime) = 0;
		virtual void Render(const float deltaTime) = 0;
	};
}

#endif // CODEX_CORE_SCENE_H
