#ifndef CODEX_SCENE_NATIVE_BEHAVIOUR_H
#define CODEX_SCENE_NATIVE_BEHAVIOUR_H

#include <sdafx.h>

#include <Engine/Scene/ECS.h>

namespace codex {
    // Forward declarations.
    class Scene;
    class NativeBehaviourComponent;

    class CODEX_API NativeBehaviour
    {
        friend class Scene;
        friend class NativeBehaviourComponent;

    protected:
        Entity                         m_Parent;
        mutable nlohmann::ordered_json m_SerializedData;

    public:
        constexpr const nlohmann::ordered_json& GetSerializedData() const noexcept { return m_SerializedData; }
        inline void                             SetOwner(const Entity entity) noexcept { m_Parent = entity; }

    public:
        virtual ~NativeBehaviour() { std::cout << "~NativeBehaviour()\n"; };

    public:
        [[nodiscard]] Entity GetPrimaryCameraEntity() noexcept { return m_Parent.m_Scene->GetPrimaryCameraEntity(); }
        [[nodiscard]] TransformComponent& GetTransform() noexcept;
        auto CreateEntity(const std::string_view tag = "default tag") { return m_Parent.m_Scene->CreateEntity(tag); }
        void RemoveEntity(Entity entity) { m_Parent.m_Scene->RemoveEntity(entity); }
        [[nodiscard]] auto GetAllEntities() { return m_Parent.m_Scene->GetAllEntities(); }
        [[nodiscard]] auto GetAllEntitiesWithTag(const std::string_view tag)
        {
            return m_Parent.m_Scene->GetAllEntitesWithTag(tag);
        }
        [[nodiscard]] auto        GetEntityCount() const noexcept { return m_Parent.m_Scene->GetEntityCount(); }
        [[nodiscard]] const auto& GetCurrentScene() const noexcept { return m_Parent.m_Scene; }
        [[nodiscard]] auto&       GetCurrentScene() noexcept { return *m_Parent.m_Scene; }

    public:
        template <typename T>
        [[nodiscard]] auto GetAllEntitiesWithComponent()
        {
            return m_Parent.m_Scene->GetAllEntitiesWithComponent<T>();
        }
        template <typename T, typename... TArgs>
        T& AddComponent(TArgs&&... args)
        {
            return m_Parent.AddComponent<T>(std::forward<TArgs>(args)...);
        }
        template <typename T>
        void RemoveComponent()
        {
            m_Parent.RemoveComponent<T>();
        }
        template <typename T>
        [[nodiscard]] T& GetComponent()
        {
            return m_Parent.GetComponent<T>();
        }
        template <typename T>
        [[nodiscard]] const T& GetComponent() const
        {
            return (NativeBehaviour*)(this)->GetComponent<T>();
        }
        template <typename T>
        [[nodiscard]] bool HasComponent() const
        {
            return m_Parent.HasComponent<T>();
        }

        // FIXME: Mark these methods protected!
        // protected:
    public:
        virtual void OnInit() = 0;
        virtual void OnUpdate(const f32 deltaTime) {}
        virtual void OnFixedUpdate(const f32 deltaTime) {}
        virtual void OnDispose() {}
        virtual void Serialize() const noexcept { m_SerializedData[typeid(*this).name()]["Id"] = -1; };
        [[nodiscard]] virtual mem::Box<NativeBehaviour> Clone() const = 0;
        [[nodiscard]] virtual object GetField(const std::string_view name) noexcept { return nullobj; }
    };
} // namespace codex

#endif // CODEX_SCENE_NATIVE_BEHAVIOUR_H
