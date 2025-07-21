#ifndef CODEX_CONCURRENCY_MUTEX_H
#define CODEX_CONCURRENCY_MUTEX_H

#include <sdafx.h>

#include <Engine/Core/CommonDef.h>
#include <Engine/Memory/Memory.h>

namespace codex::cc {
template <typename T>
    class Sync
    {
    public:
        /* @brief Provides RAII ownership mechanism for @ref Sync objects much like @ref std::scoped_lock. */
        class ScopedGuard;

    private:
        T          m_Object;
        std::mutex m_Sync;
#ifdef CX_CONFIG_DEBUG
        std::thread::id m_CurrentOwningThreadID;
#endif

    public:
        template <typename... TArgs>
        inline Sync(TArgs&&... args)
            : m_Object(std::forward<TArgs>(args)...)
        {
        }
        inline Sync(const Sync&)                = delete;
        inline Sync& operator=(const Sync&)     = delete;
        inline Sync(Sync&&) noexcept {

        }
        inline Sync& operator=(Sync&&) noexcept = delete;

    public:
        [[nodiscard]] inline ScopedGuard operator->() noexcept
        {
            return ScopedGuard{ *this };
        }
        [[nodiscard]] inline const ScopedGuard operator->() const noexcept
        {
            return const_cast<Sync<T>*>(this)->operator->();
        }
        [[nodiscard]] inline ScopedGuard operator*() noexcept
        {
            return ScopedGuard{ *this };
        }
        [[nodiscard]] inline const ScopedGuard operator*() const noexcept
        {
            return const_cast<Sync<T>*>(this)->operator*();
        }

    private:
        inline void TryLock()
        {
#ifdef CX_CONFIG_DEBUG
            if (std::this_thread::get_id() == m_CurrentOwningThreadID) {
                throw std::runtime_error("Sync: Same thread tried locking twice.");
            }
            m_CurrentOwningThreadID = std::this_thread::get_id();
#endif
            m_Sync.lock();
        }
        inline void TryUnlock()
        {
#ifdef CX_CONFIG_DEBUG
            if (m_CurrentOwningThreadID != std::this_thread::get_id()) {
                throw std::runtime_error("Sync: A non-owning thread tried unlocking.");
            }
#endif
            m_Sync.unlock();
#ifdef CX_CONFIG_DEBUG
            m_CurrentOwningThreadID = std::thread::id{};
#endif
        }

    public:
        [[nodiscard]] inline ScopedGuard Lock() noexcept
        {
            return ScopedGuard{ *this };
        }
        [[nodiscard]] inline const ScopedGuard Lock() const noexcept
        {
            return const_cast<Sync<T>*>(this)->Lock();
        }
        inline void Swap(Sync& other) noexcept
        {
            std::scoped_lock this_guard{ m_Sync };
            std::scoped_lock other_guard{ other.m_Sync };
            std::swap(m_Object, other.m_Object);
        }

    public:
        template <typename... TArgs>
        [[nodiscard]] static inline Sync New(TArgs&&... args) noexcept
        {
            return Sync{ std::forward<TArgs>(args)... };
        }
    };

    template <typename T>
    class Sync<T>::ScopedGuard
    {
        friend class Sync<T>;

    private:
        Sync<T>& m_Sync;

    private:
        inline ScopedGuard(Sync<T>& sync) noexcept
            : m_Sync(sync)
        {
            m_Sync.TryLock();
        }
        inline ScopedGuard(const ScopedGuard&)                = delete;
        inline ScopedGuard& operator=(const ScopedGuard&)     = delete;
        inline ScopedGuard(ScopedGuard&&)                     = delete;
        inline ScopedGuard& operator=(ScopedGuard&&) noexcept = delete;

    public:
        inline ~ScopedGuard() noexcept
        {
            m_Sync.TryUnlock();
        }

    public:
        [[nodiscard]] inline T* operator->() noexcept
        {
            return std::addressof(m_Sync.m_Object);
        }
        [[nodiscard]] inline const T* operator->() const noexcept
        {
            return const_cast<ScopedGuard*>(this)->operator->();
        }
        [[nodiscard]] inline T& operator*() noexcept
        {
            return m_Sync.m_Object;
        }
        [[nodiscard]] inline const T& operator*() const noexcept
        {
            return const_cast<ScopedGuard*>(this)->operator*();
        }
    };

    template <typename T>
    using SharedSync = mem::Shared<Sync<T>>;

    template <typename T, typename... TArgs>
    constexpr SharedSync<T> NewSharedSync(TArgs&&... args)
    {
        return mem::Shared<Sync<T>>{ new Sync<T>{ std::forward<TArgs>(args)... } };
    }
} // namespace codex::cc

#endif // CODEX_CONCURRENCY_MUTEX_H
