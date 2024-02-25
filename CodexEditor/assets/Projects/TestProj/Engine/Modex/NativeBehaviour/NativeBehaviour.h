#ifndef CODEX_SCENE_NATIVE_BEHAVIOUR_H
#define CODEX_SCENE_NATIVE_BEHAVIOUR_H

#include <sdafx.h>

#include "../Scene/ECS.h"

namespace codex {
    // Forward declarations.
    class Scene;
    class NativeBehaviourComponent;

    enum class FieldType : u8
    {
        U8,
        I8,
        U16,
        I16,
        U32,
        I32,
        U64,
        I64,
        USize,
        Boolean,
        F32,
        F64,
        F128,
        Vector2f,
        Vector2,
        Vector3f,
        Vector3,
        Vector4f,
        Vector4,
        ObjectRef,
        CString,
        StdString
    };

    class CODEX_API NativeBehaviour
    {
        friend class Scene;
        friend class NativeBehaviourComponent;

    protected:
        Entity                 m_Owner;
        nlohmann::ordered_json m_SerializedData;

    public:
        constexpr const nlohmann::ordered_json& GetSerializedData() const noexcept { return m_SerializedData; }
        inline void                             SetOwner(const Entity entity) noexcept { m_Owner = entity; }

    public:
        auto CreateEntity(const std::string_view tag = "default tag") { return m_Owner.m_Scene->CreateEntity(tag); }
        void RemoveEntity(codex::Entity entity) { m_Owner.m_Scene->RemoveEntity(entity); }
        auto GetAllEntities() { return m_Owner.m_Scene->GetAllEntities(); }
        auto GetAllEntitiesWithTag(const std::string_view tag) { return m_Owner.m_Scene->GetAllEntitesWithTag(tag); }
        auto GetEntityCount() const noexcept { return m_Owner.m_Scene->GetEntityCount(); }
        const auto& GetCurrentScene() const noexcept { return m_Owner.m_Scene; }
        auto& GetCurrentScene() noexcept { return *m_Owner.m_Scene; }

    public:
        template <typename T>
        auto GetAllEntitiesWithComponent()
        {
            return m_Owner.m_Scene->GetAllEntitiesWithComponent<T>();
        }
        template <typename T, typename... TArgs>
        T& AddComponent(TArgs&&... args)
        {
            return m_Owner.AddComponent<T>(std::forward<TArgs>(args)...);
        }
        template <typename T>
        void RemoveComponent()
        {
            m_Owner.RemoveComponent<T>();
        }
        template <typename T>
        T& GetComponent()
        {
            return m_Owner.GetComponent<T>();
        }
        template <typename T>
        const T& GetComponent() const
        {
            return (NativeBehaviour*)(this)->GetComponent<T>();
        }
        template <typename T>
        bool HasComponent() const
        {
            return m_Owner.HasComponent<T>();
        }

    public:
        virtual ~NativeBehaviour() = default;

        // FIXME: Mark these methods protected!
        // protected:
    public:
        virtual void   Init() = 0;
        virtual void   Update(const f32 deltaTime) {}
        virtual void   Destroy() {}
        virtual void   Serialize() { m_SerializedData[typeid(this).name()]["Id"] = -1; };
        virtual object GetField(const std::string_view name) { return nullobj; }
    };
} // namespace codex

#endif // CODEX_SCENE_NATIVE_BEHAVIOUR_H
