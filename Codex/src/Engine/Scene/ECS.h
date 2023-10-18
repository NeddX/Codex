#ifndef CODEX_SCENE_ECS_H
#define CODEX_SCENE_ECS_H

#include <sdafx.h>

#include "Scene.h"
#include "../Core/Geomtryd.h"
#include "../Renderer/SpriteBatchRenderer.h"
#include "Components.h"

namespace codex {
	class Entity
	{
		friend class Scene;

	private:
		entt::entity m_Handle { 0 };
		Scene* m_Scene = nullptr;

	public:
		Entity() = default;
		Entity(const entt::entity entity, Scene* scene) :
			m_Handle(entity), m_Scene(scene)
		{

		}
		Entity(const u32 entity, Scene* scene) :
			m_Handle((entt::entity)entity), m_Scene(scene)
		{

		}
	public:
		inline u32 GetId() const
			{ return (u32)(m_Handle); }
		operator bool() const
			{ return m_Handle != entt::entity { 0 }; }

	public:
		template<typename T, typename... TArgs>
		T& AddComponent(TArgs&&... args)
		{
			CX_ASSERT(!m_Scene->m_Registry.any_of<T>(m_Handle), "Entity already has that component.");
			auto& c = m_Scene->m_Registry.emplace<T>(m_Handle, std::forward<TArgs>(args)...);
			c.m_Parent = std::make_unique<Entity>(m_Handle, m_Scene);
			c.Start();
			return c;
		}
		template<typename T>
		void RemoveComponent()
		{
			CX_ASSERT(m_Scene->m_Registry.any_of<T>(m_Handle), "Entity does not have the component to remove.");
			m_Scene->m_Registry.remove<T>(m_Handle);
		}
		template<typename T>
		T& GetComponent()
		{
			CX_ASSERT(m_Scene->m_Registry.any_of<T>(m_Handle), "Entity does not have the component to retrieve.");
			return m_Scene->m_Registry.get<T>(m_Handle);
		}
		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.any_of<T>(m_Handle);
		}
	};

	/*
	class EntityManager
	{
		friend class Entity;

	private:
		entt::registry m_Registry;

	public:
		EntityManager()
		{

		}

	public:
		Entity CreateEntity(const std::string_view entityTag = "default tag")
		{
			Entity entity(m_Registry.create(), this);
			entity.AddComponent<TransformComponent>();
			entity.AddComponent<TagComponent>(entityTag);
			return entity;
		}
		void RemoveEntity(const Entity entity)
		{
			m_Registry.destroy(entity.m_Handle);
		}
		template<typename T>
		std::vector<Entity> GetAllEntitiesWithComponent()
		{
			auto view = m_Registry.view<T>();
			std::vector<Entity> entities;
			entities.reserve(view.size_hint());
			for (auto& e : view)
				entities.emplace_back(e, this);
			return entities;
		}
		std::vector<Entity> GetAllEntitesWithTag(const std::string_view tag)
		{
			auto view = m_Registry.view<TagComponent>();
			std::vector<Entity> entities;
			entities.reserve(view.size_hint());
			for (auto& e : view)
			{
				Entity entity = { e, this };
				if (entity.GetComponent<TagComponent>().tag == tag)
					entities.push_back(entity);
			}
			return entities;
		}
	};
	*/
}
#endif // CODEX_SCENE_ECS_H
