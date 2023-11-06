#ifndef CODEX_SCENE_NATIVE_BEHAVIOUR_H
#define CODEX_SCENE_NATIVE_BEHAVIOUR_H

#include <sdafx.h>

#include "../Scene/ECS.h"

namespace codex {
    // Forward declarations.
    class Scene;

    class DynamicLibraryException : public CodexException
    {
        using CodexException::CodexException;

    public:
        inline const char* default_message() const noexcept override { return "Failed to load dynamic library."; }
    };

#ifdef CX_PLATFORM_WINDOWS
    using DLib = HINSTANCE;
#elif defined(CX_PLATFORM_UNIX)
    using DLib = void*;

    DLib LoadLibrary(const char* filePath)
    {
        DLib handle = dlopen(filePath, RTLD_LAZY);
        if (!handle)
            CX_THROW(DynamicLibraryException, "Failed to load '{}'.", filePath);
    }
#endif

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
