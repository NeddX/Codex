#ifndef CODEX_SCENE_ECS_H
#define CODEX_SCENE_ECS_H

#include <sdafx.h>

#include "Scene.h"

namespace codex {
    // Forward declerations.
    class Serializer;
    class NativeBehaviour;
    class SpriteRendererComponent;

    class CODEX_API Entity
    {
        friend class Serializer;
        friend class Scene;
        friend class SpriteRendererComponent;
        friend class NativeBehaviour;

    public:
        using HandleType       = entt::id_type;
        using SingedHandleType = std::make_signed_t<HandleType>();

    private:
        entt::entity m_Handle{ entt::null };
        Scene*       m_Scene = nullptr;

    public:
        constexpr Entity() = default;
        Entity(const entt::entity entity, Scene* scene)
            : m_Handle(entity)
            , m_Scene(scene)
        {
        }
        Entity(const HandleType entity, Scene* scene)
            : m_Handle((entt::entity)entity)
            , m_Scene(scene)
        {
        }

    public:
        [[nodiscard]] static constexpr Entity None() noexcept { return Entity(); }

    public:
        [[nodiscard]] operator HandleType() const noexcept { return (HandleType)m_Handle; }

    public:
        [[nodiscard]] inline HandleType GetId() const noexcept { return (HandleType)(m_Handle); }
        [[nodiscard]] inline bool       IsValid() const noexcept { return m_Scene->m_Registry.valid(m_Handle); }
        [[nodiscard]] operator bool() const noexcept { return m_Handle != entt::entity{ entt::null } && IsValid(); }
        [[nodiscard]] bool operator==(const Entity& other) const { return other.m_Handle == m_Handle; }

    public:
        template <typename T, typename... TArgs>
        T& AddComponent(TArgs&&... args)
        {
            CX_ASSERT(!m_Scene->m_Registry.any_of<T>(m_Handle), "Entity already has that component.");
            auto& c    = m_Scene->m_Registry.emplace<T>(m_Handle, std::forward<TArgs>(args)...);
            c.m_Parent = Entity(m_Handle, m_Scene);
            c.OnInit();
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
        const T& GetComponent() const
        {
            return ((Entity*)this)->GetComponent<T>();
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
