#ifndef CODEX_CORE_SCENE_H
#define CODEX_CORE_SCENE_H

#include <sdafx.h>

#include "Memory.h"
#include "Camera.h"
#include "../Scene/ECS/ECS.h"
#include "../Renderer/Renderer.h"
#include "../Scene/SpriteSheet.h"
#include "../Core/ResourceHandler.h"

namespace Codex
{
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
		//Entity* m_ActiveEntity;
		
	private:
		bool m_Running;

	public:
		Scene(Renderer* renderer, int width, int height);
		virtual ~Scene() = default;

	public:
		inline Camera* GetCamera() const { return m_Camera.get(); }
		inline Entity CreateEntity(const std::string& tag = "default tag") { return m_Manager.CreateEntity(tag); }
		inline void RemoveEntity(Entity entity) { m_Manager.RemoveEntity(entity); }
		inline void RemoveEntity(uint32_t entity) { m_Manager.RemoveEntity(Entity((entt::entity)(entity), &m_Manager)); }

	public:
		void OnWindowResize_Event(int newWidth, int newHeight)
		{
			m_Camera->SetWidth(newWidth);
			m_Camera->SetHeight(newHeight);
		}

	private:
		virtual void Init();

	public:
		virtual void Start()					= 0;
		virtual void Update(float deltaTime)	= 0;
		//virtual void SceneEditor();
		//virtual void Editor();
	};
}

#endif // CODEX_CORE_SCENE_H
