#ifndef CODEX_CONCURRENCY_MUTEX_H
#define CODEX_CONCURRENCY_MUTEX_H

#include <sdafx.h>

#include <Engine/Core/CommonDef.h>

namespace codex::cc {
    // TODO: Rename to Sync and create Codex Exceptions for CC stuff and handle them here.
    // TODO: Like Sync in mgrd, add debug checking for the same thread trying to lock twice.
    template <typename T>
    class Mutex
    {
    public:
        class ScopedGuard;

    private:
        T          m_Object;
        std::mutex m_Mutex;

    public:
        template <typename... TArgs>
        constexpr Mutex(TArgs&&... args)
            : m_Object(std::forward<TArgs>(args)...)
        {
        }
        constexpr Mutex(const Mutex<T>& other) noexcept               = delete;
        constexpr Mutex<T>& operator=(const Mutex<T>& other) noexcept = delete;
        constexpr Mutex(Mutex<T>&& other) noexcept                    = delete;
        constexpr Mutex<T>& operator=(Mutex<T>&& other) noexcept      = delete;
        constexpr ~Mutex() noexcept                                   = default;

    public:
        [[nodiscard]] constexpr ScopedGuard       operator->() noexcept { return ScopedGuard(*this); }
        [[nodiscard]] constexpr const ScopedGuard operator->() const noexcept
        {
            return const_cast<Mutex<T>*>(this)->operator->();
        }
        [[nodiscard]] constexpr ScopedGuard       operator*() noexcept { return ScopedGuard(*this); }
        [[nodiscard]] constexpr const ScopedGuard operator*() const noexcept
        {
            return const_cast<Mutex<T>*>(this)->operator*();
        }

    private:
        inline void TryLock() { m_Mutex.lock(); }
        inline void Unlock() { m_Mutex.unlock(); }

    public:
        [[nodiscard]] inline ScopedGuard Lock() { return ScopedGuard(*this); }
        [[nodiscard]] inline ScopedGuard Lock(std::string func, std::string file, int line)
        {
            return ScopedGuard(*this, func, file, line);
        }
        [[nodiscard]] inline const ScopedGuard Lock(std::string func, std::string file, int line) const
        {
            return const_cast<Mutex<T>*>(this)->Lock(func, file, line);
        }
        [[nodiscard]] inline const ScopedGuard Lock() const { return const_cast<Mutex<T>*>(this)->Lock(); }

    public:
        template <typename... TArgs>
        [[nodiscard]] static inline Mutex New(TArgs&&... args) noexcept
        {
            return Mutex<T>(std::forward<TArgs>(args)...);
        }
    };

    template <typename T>
    class Mutex<T>::ScopedGuard
    {
        friend class Mutex<T>;

    private:
        Mutex<T>& m_Mutex;

    private:
        constexpr ScopedGuard(Mutex<T>& mutex)
            : m_Mutex(mutex)
        {
            m_Mutex.TryLock();
        }

    public:
        constexpr ~ScopedGuard() { m_Mutex.Unlock(); }

    public:
        constexpr ScopedGuard(const ScopedGuard&) noexcept            = delete;
        constexpr ScopedGuard& operator=(const ScopedGuard&) noexcept = delete;

    public:
        [[nodiscard]] constexpr T*       operator->() noexcept { return std::addressof(m_Mutex.m_Object); }
        [[nodiscard]] constexpr const T* operator->() const noexcept
        {
            return const_cast<ScopedGuard*>(this)->operator->();
        }
        [[nodiscard]] constexpr T&       operator*() noexcept { return m_Mutex.m_Object; }
        [[nodiscard]] constexpr const T& operator*() const noexcept
        {
            return const_cast<ScopedGuard*>(this)->operator*();
        }
    };
} // namespace codex::cc

#endif // CODEX_CONCURRENCY_MUTEX_H
