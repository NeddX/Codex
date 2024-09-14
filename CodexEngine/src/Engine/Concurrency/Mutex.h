#ifndef CODEX_CONCURRENCY_MUTEX_H
#define CODEX_CONCURRENCY_MUTEX_H

#include <sdafx.h>

#include <Engine/Core/CommonDef.h>

namespace codex::cc {
    template <typename T>
    class Mutex
    {
    public:
        class ScopedGuard;

    private:
        std::atomic<bool> m_Locked = false;
        T                 m_Object;

    public:
        template <typename... TArgs>
        constexpr Mutex(TArgs&&... args)
            : m_Object(std::forward<TArgs>(args)...)
        {
        }
        constexpr Mutex(const Mutex<T>& other)
            : m_Object(other.m_Object)
            , m_Locked(other.m_Locked.load())
        {
        }
        constexpr Mutex<T>& operator=(const Mutex<T>& other)
        {
            Mutex<T>{ other }.Swap(*this);
            return *this;
        }
        constexpr Mutex(Mutex<T>&& other) noexcept
            : m_Object(std::move(other.m_Object))
            , m_Locked(other.m_Locked.load())
        {
        }
        constexpr Mutex<T>& operator=(Mutex<T>&& other) noexcept
        {
            Mutex<T>{ std::move(other) }.Swap(*this);
            return *this;
        }
        constexpr ~Mutex() noexcept { TryLock(); }

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
        inline void TryLock() noexcept
        {
            while (m_Locked)
                ;
            m_Locked = true;
        }
        inline void Unlock() noexcept { m_Locked = false; }

    public:
        [[nodiscard]] inline ScopedGuard Lock() noexcept { return ScopedGuard(*this); }
        [[nodiscard]] inline ScopedGuard Lock(std::string func, std::string file, int line) noexcept
        {
            return ScopedGuard(*this, func, file, line);
        }
        [[nodiscard]] inline const ScopedGuard Lock(std::string func, std::string file, int line) const noexcept
        {
            return const_cast<Mutex<T>*>(this)->Lock(func, file, line);
        }
        [[nodiscard]] inline const ScopedGuard Lock() const noexcept { return const_cast<Mutex<T>*>(this)->Lock(); }
        inline void                            Swap(Mutex<T>& other) noexcept
        {
            std::swap(m_Object, other.m_Object);

            const auto temp = other.m_Locked.load();
            other.m_Locked.store(m_Locked.load());
            m_Locked.store(temp);
        }

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
        Mutex<T>&   m_Mutex;
        std::string func;
        std::string file;
        int         line;
        bool        yes = false;

    private:
        constexpr ScopedGuard(Mutex<T>& mutex, std::string func, std::string file, int line)
            : m_Mutex(mutex)
        {
            this->func = func;
            this->file = file;
            this->line = line;
            yes        = true;
            lgx::Get("engine").Log(lgx::Level::Info, "Mutex locked. \n\tFile: {}\n\tFunc: {}\n\tLine: {}", file, func,
                                   line);
        }
        constexpr ScopedGuard(Mutex<T>& mutex) noexcept
            : m_Mutex(mutex)
        {
            m_Mutex.TryLock();
        }

    public:
        constexpr ~ScopedGuard() noexcept
        {
            m_Mutex.Unlock();
            if (yes)
            {
                lgx::Get("engine").Log(lgx::Level::Info, "Mutex unlocked. \n\tFile: {}\n\tFunc: {}\n\tLine: {}", file,
                                       func, line);
            }
        }

    public:
        ScopedGuard(const ScopedGuard&) noexcept            = delete;
        ScopedGuard& operator=(const ScopedGuard&) noexcept = delete;

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
