#ifndef CODEX_SCENE_ECS_H
#define CODEX_SCENE_ECS_H

#include <sdafx.h>

#include "../../Core/Geomtryd.h"
#include "../../Renderer/SpriteBatchRenderer.h"
#include "Components.h"

namespace Codex
{
	// Forward declarations
	class EntityManager;

	class Entity
	{
		friend class EntityManager;

	private:
		entt::entity m_Handle { 0 };
		EntityManager* m_Manager = nullptr;

	public:
		Entity() = default;
		Entity(entt::entity entity, EntityManager* manager) :
			m_Handle(entity), m_Manager(manager)
		{

		}
	public:
		inline uint32_t GetID() const { return (uint32_t)(m_Handle); }
		operator bool() const { return m_Handle != entt::entity { 0 }; }

	public:
		template<typename T, typename... TArgs>
		T& AddComponent(TArgs&&... args)
		{
			// TODO: Add own assertion macro
			return m_Manager->m_Registry.emplace<T>(m_Handle, std::forward<TArgs>(args)...);
		}
		template<typename T>
		void RemoveComponent()
		{
			m_Manager->m_Registry.remove<T>(m_Handle);
		}
		template<typename T>
		T& GetComponent()
		{
			return m_Manager->m_Registry.get<T>(m_Handle);
		}
		template<typename T>
		bool HasComponent()
		{
			return m_Manager->m_Registry.any<T>(m_Handle);
		}
	};

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
		Entity CreateEntity(const std::string& entityTag = "default tag")
		{
			Entity entity(m_Registry.create(), this);
			entity.AddComponent<TransformComponent>();
			entity.AddComponent<TagComponent>(entityTag);
			return entity;
		}
		void RemoveEntity(Entity entity)
		{
			m_Registry.destroy(entity.m_Handle);
		}
		template<typename T>
		std::vector<Entity> GetAllEntitiesWithComponent()
		{
			auto view = m_Registry.view<T>();
			std::vector<Entity> entities;
			entities.reserve(view.size());
			for (auto& e : view)
				entities.emplace_back(e, this);
			return entities;
		}
	};
}

/*
namespace Codex
{
	// Forward declerations
	class Entity;
	class IComponent;

	const static size_t maxComponents = 30;

	static size_t GetNewComponentID()
	{
		static size_t id = 0;
		return id++;
	}

	template<typename T>
	static size_t GetComponentID()
	{
		static size_t id = GetNewComponentID();
		return id;
	}

	typedef std::array<IComponent*, maxComponents> ComponentList;
	typedef std::bitset<maxComponents> ComponentBitSet;

	class IComponent
	{
		friend class Entity;

	protected:
		Entity* m_Parent = nullptr;

	private:
		bool m_Active = true;
		bool m_Render = true;
		bool m_ToBeRemoved = false;

	public:
		IComponent()			= default;
		virtual ~IComponent()	= default;

	public:
		inline bool GetActive() const				{ return m_Active; }
		inline bool GetVisible() const				{ return m_Render; }
		inline void SetActive(bool active)			{ m_Active = active; }
		inline void SetVisible(bool visible)		{ m_Render = visible; }

	public:
		virtual void Start()												= 0;
		virtual void Update(float deltaTime)								= 0;
		virtual void Render(float deltaTime, SpriteBatchRenderer* renderer) = 0;
	};

	class Entity
	{
	protected:
		std::string m_Name;

	private:
		std::vector<IComponent*> m_Components;
		ComponentList m_ComponentList;
		ComponentBitSet m_ComponentBitSet;
		bool m_Active = true;
		bool m_Render = true;
	
	public:
		Entity(const std::string& name);
		~Entity();

	public:
		inline bool GetActive() const			{ return m_Active; }
		inline bool GetVisible() const			{ return m_Render; }
		inline void SetActive(bool active)		{ m_Active = active; }
		inline void SetVisible(bool visible)	{ m_Render = visible; }
		inline void Refresh()
		{
			m_Components.erase(std::remove_if(m_Components.begin(), m_Components.end(), [](const IComponent* c)
				{
					return c->m_ToBeRemoved;
				}));
		}
		template<typename T, std::enable_if_t<std::is_base_of<IComponent, T>::value, int> = 0>
		inline T& GetComponent()
		{
			auto* c = m_ComponentList[GetComponentID<T>()];
			return (T&)(*c);
			//return *((T)(m_ComponentList[GetComponentID<T>()]));
		}
		template<typename T, std::enable_if_t<std::is_base_of<IComponent, T>::value, int> = 0>
		inline bool HasComponent()
		{
			return m_ComponentBitSet[GetComponentID<T>()];
		}
		template<typename T, typename... TArgs, std::enable_if_t<std::is_base_of<IComponent, T>::value, int> = 0>
		inline T& AddComponent(TArgs&&... args)
		{
			T* c = new T(std::forward<TArgs>(args)...);
			const size_t id = GetComponentID<T>();
			c->m_Parent = this;
			c->Start();
			m_Components.push_back(c);
			m_ComponentList[id] = c;
			m_ComponentBitSet[id] = true;
			return *c;
		}
		template<typename T, std::enable_if_t<std::is_base_of<IComponent, T>::value, int> = 0>
		inline bool RemoveComponent()
		{
			if (HasComponent<T>())
			{
				const size_t id = GetComponentID<T>();
				T* c = m_ComponentList[id];
				c->m_ToBeRemoved = true;
				Refresh();
				m_ComponentList[id] = nullptr;
				m_ComponentBitSet[id] = false;
				delete c;
				return true;
			}
			return false;
		}

	public:
		virtual void Start();
		virtual void Update(float deltaTime);
		virtual void Render(float deltaTime, SpriteBatchRenderer* renderer);
		virtual void Editor();
	};
}
*/
#endif // CODEX_SCENE_ECS_H