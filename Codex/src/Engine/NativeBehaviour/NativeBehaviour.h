#ifndef CODEX_SCENE_NATIVE_BEHAVIOUR_H
#define CODEX_SCENE_NATIVE_BEHAVIOUR_H

#include <sdafx.h>

#include "../Scene/ECS.h"

namespace codex {
    // Forward declarations.
    class Scene;

    class NativeBehaviour
    {
        friend class Scene;

    protected:
        Entity m_Owner;

    public:
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
        bool HasComponent()
        {
            return m_Owner.HasComponent<T>();
        }

    public:
        NativeBehaviour()          = default;
        virtual ~NativeBehaviour() = default;

        // FIXME: Mark these methods protected!
        // protected:
    public:
        virtual void Init() {};
        virtual void Update(const f32 deltaTime) {}
        virtual void Destroy() {}
    };
} // namespace codex

#endif // CODEX_SCENE_NATIVE_BEHAVIOUR_H
