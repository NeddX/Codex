#ifndef CODEX_MEMORY_SHARED_H
#define CODEX_MEMORY_SHARED_H

#include <sdafx.h>

namespace codex::mem {
    // Forward decleratons.
    template <typename T>
    class Ref;

    template <typename T>
    class Shared
    {
        template <typename U>
        friend class Shared; // We don't ask questions here.
        friend class Ref<T>;

    private:
        using BaseType  = typename std::remove_extent<T>::type;
        using Pointer   = BaseType*;
        using Reference = BaseType&;

    private:
        Pointer             m_Ptr      = nullptr;
        std::atomic<usize>* m_RefCount = nullptr;

    public:
        Shared() = default;
        Shared(Pointer&& rawPtr) : m_Ptr(rawPtr), m_RefCount(new std::atomic<usize>(1)) { rawPtr = nullptr; }
        Shared(const Shared<T>& other) noexcept
        {
            m_Ptr      = other.m_Ptr;
            m_RefCount = other.m_RefCount;

            if (m_RefCount)
                ++(*m_RefCount);
        };
        Shared(Shared<T>&& other) noexcept
        {
            if (this == &other)
                return;

            m_Ptr            = other.m_Ptr;
            m_RefCount       = other.m_RefCount;
            other.m_Ptr      = nullptr;
            other.m_RefCount = nullptr;
        }
        ~Shared() { Drop(true); }

    private:
        inline void Drop(const bool destructing = false)
        {
            if (m_Ptr)
            {
                if (--(*m_RefCount) == 0)
                {
                    if (std::is_array_v<T>)
                        delete[] m_Ptr;
                    else
                        delete m_Ptr;

                    delete m_RefCount;
                }
                m_Ptr      = nullptr;
                m_RefCount = nullptr;
            }
        }

    public:
        constexpr Pointer       Get() noexcept { return m_Ptr; }
        constexpr const Pointer Get() const noexcept { return m_Ptr; }
        inline Ref<T>           AsRef() noexcept { return *this; }
        inline const Ref<T>     AsRef() const noexcept { return *this; }

    public:
        constexpr                 operator bool() const noexcept { return m_Ptr; }
        constexpr Pointer         operator->() noexcept { return m_Ptr; }
        constexpr const Pointer   operator->() const noexcept { return m_Ptr; }
        constexpr Reference       operator*() noexcept { return *m_Ptr; }
        constexpr const Reference operator*() const noexcept { return *m_Ptr; }
        inline Shared<T>&         operator=(const Shared<T>& other) noexcept
        {
            if (this == &other)
                return *this;

            Drop();

            m_Ptr      = other.m_Ptr;
            m_RefCount = other.m_RefCount;

            if (m_RefCount)
                ++m_RefCount;
            return *this;
        }
        inline Shared<T>& operator=(Shared<T>&& other) noexcept
        {
            if (this == &other)
                return *this;

            Drop();

            m_Ptr            = other.m_Ptr;
            m_RefCount       = other.m_RefCount;
            other.m_Ptr      = nullptr;
            other.m_RefCount = nullptr;
            return *this;
        }
        inline Shared<T>& operator=(const T*&& ptr) noexcept
        {
            if (m_Ptr == ptr)
                return *this;

            Drop();

            m_RefCount = new std::atomic<usize>(1);
            m_Ptr      = ptr;
            ptr        = nullptr;
            return *this;
        }

    public:
        inline Shared<T>& Reset(Pointer&& ptr = nullptr)
        {
            if (ptr == m_Ptr)
                return *this;

            Drop();

            m_RefCount = new std::atomic<usize>(1);
            m_Ptr      = ptr;
            ptr        = nullptr;
            return *this;
        }
        inline Shared<T>& Swap(Shared<T>& other) noexcept
        {
            std::swap(m_Ptr, other.m_Ptr);
            std::swap(m_RefCount, other.m_RefCount);
        }

    public:
        template <typename U>
        Shared<U> As()
        {
            Shared<U> cast_ptr;
            cast_ptr.m_Ptr      = (typename Shared<U>::Pointer)m_Ptr;
            cast_ptr.m_RefCount = m_RefCount;
            ++(*cast_ptr.m_RefCount);
            return cast_ptr;
        }

    public:
        template <typename... TArgs>
        static inline Shared<T> New(TArgs&&... args)
        {
            return std::move(Shared<T>{ new T(std::forward<TArgs>(args)...) });
        }
        static inline Shared<T> From(Pointer&& rawPtr)
        {
            Shared<T> obj = std::move(rawPtr);
            return std::move(obj);
        }
    };
} // namespace codex::mem

#endif // CODEX_MEMORY_BOX_H
