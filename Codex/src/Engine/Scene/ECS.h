#ifndef CODEX_SCENE_ECS_H
#define CODEX_SCENE_ECS_H

#include <sdafx.h>

#include "Scene.h"

namespace codex {
    // Forward declerations.
    class Serializer;
    class SpriteRendererComponent;
    class NativeBehaviour;

    class CODEX_API Entity
    {
        friend class Serializer;
        friend class Scene;
        friend class SpriteRendererComponent;
        friend class NativeBehaviour;

    private:
        entt::entity m_Handle{ entt::null };
        Scene*       m_Scene = nullptr;


    public:
        Entity() = default;
        Entity(const entt::entity entity, Scene* scene) : m_Handle(entity), m_Scene(scene) {}
        Entity(const u32 entity, Scene* scene) : m_Handle((entt::entity)entity), m_Scene(scene) {}

    public:
        static inline Entity None() { return Entity(); }

    public:
        inline i32 GetId() const { return (i32)(m_Handle); }
        inline bool IsValid() const noexcept { return m_Scene->m_Registry.valid(m_Handle); }
        operator bool() const { return m_Handle != entt::entity{ entt::null } && IsValid(); }
        bool operator==(const Entity& other) const { return other.m_Handle == m_Handle; }

    public:
        template <typename T, typename... TArgs>
        T& AddComponent(TArgs&&... args)
        {
            CX_ASSERT(!m_Scene->m_Registry.any_of<T>(m_Handle), "Entity already has that component.");
            auto& c    = m_Scene->m_Registry.emplace<T>(m_Handle, std::forward<TArgs>(args)...);
            c.m_Parent = Entity(m_Handle, m_Scene);
            c.Start();
            return c;
        }
        template <typename T>
        void RemoveComponent()
        {
            CX_ASSERT(m_Scene->m_Registry.any_of<T>(m_Handle), "Entity does not have the component to remove.");
            m_Scene->m_Registry.remove<T>(m_Handle);
        }
        template <typename T>
        T& GetComponent()
        {
            CX_ASSERT(m_Scene->m_Registry.any_of<T>(m_Handle), "Entity does not have the component to retrieve.");
            return m_Scene->m_Registry.get<T>(m_Handle);
        }
        template <typename T>
        bool HasComponent() const
        {
            return m_Scene->m_Registry.any_of<T>(m_Handle);
        }

    public:
        friend void to_json(nlohmann::ordered_json& j, const Entity& entity);
        friend void from_json(const nlohmann::ordered_json& j, Entity& entity);
    };
} // namespace codex
#endif // CODEX_SCENE_ECS_H
