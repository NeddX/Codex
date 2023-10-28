#ifndef CODEX_SCENE_NATIVE_BEHAVIOUR_H
#define CODEX_SCENE_NATIVE_BEHAVIOUR_H

#include <sdafx.h>

#include "ECS.h"

namespace codex {
    class Scene;

    class NativeBehaviour
    {
        friend class Scene;

    private:
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
        virtual void Init() = 0;
        virtual void Update(const f32 deltaTime) {}
        virtual void Destroy() {}
    };
} // namespace codex

#endif // CODEX_SCENE_NATIVE_BEHAVIOUR_H
