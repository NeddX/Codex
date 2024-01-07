#ifndef CODEX_MEMORY_BOX_H
#define CODEX_MEMORY_BOX_H

#include <sdafx.h>

namespace codex {
    template <typename T>
    class Box
    {
    private:
        using BaseType = typename std::remove_extent<T>::type;

    private:
        BaseType* m_Ptr = nullptr;

    public:
        Box() = default;
        Box(BaseType*&& rawPtr) noexcept : m_Ptr(rawPtr) { rawPtr = nullptr; }
        Box(const Box<T>& other) noexcept = delete;
        Box(Box<T>&& other) noexcept
        {
            if (this == &other)
                return;

            m_Ptr       = other.m_Ptr;
            other.m_Ptr = nullptr;
        }
        ~Box() { Drop(true); }

    private:
        inline void Drop(const bool destructing = false)
        {
            if (m_Ptr)
            {
                if (std::is_array_v<T>)
                    delete[] m_Ptr;
                else
                    delete m_Ptr;
                m_Ptr = nullptr;
            }
        }

    public:
        constexpr T*       Get() noexcept { return m_Ptr; }
        constexpr const T* Get() const noexcept { return m_Ptr; }

    public:
        explicit constexpr        operator bool() const noexcept { return m_Ptr; }
        constexpr BaseType*       operator->() noexcept { return m_Ptr; }
        constexpr const BaseType* operator->() const noexcept { return m_Ptr; }
        constexpr BaseType&       operator*() noexcept { return *m_Ptr; }
        constexpr const BaseType& operator*() const noexcept { return *m_Ptr; }
        inline Box<T>&            operator=(const Box<T>& other) noexcept = delete;
        inline Box<T>&            operator=(Box<T>&& other) noexcept
        {
            if (this == &other)
                return *this;

            Drop();

            m_Ptr       = other.m_Ptr;
            other.m_Ptr = nullptr;
            return *this;
        }
        inline Box<T>& operator=(T*&& ptr) noexcept
        {
            if (ptr != m_Ptr)
                Drop();

            m_Ptr = ptr;
            ptr   = nullptr;
            return *this;
        }

    public:
        inline Box<T>& Reset(BaseType*&& ptr = nullptr)
        {
            if (ptr == m_Ptr)
                return *this;

            Drop();

            m_Ptr = ptr;
            ptr   = nullptr;
            return *this;
        }

    public:
        template <typename... TArgs>
        static inline Box<T> New(TArgs&&... args)
        {
            return std::move(Box<T>(new T{ std::forward<TArgs>(args)... }));
        }
        static inline Box<T> From(BaseType*&& rawPtr) noexcept
        {
            Box<T> obj = std::move(rawPtr);
            return std::move(obj);
        }
    };
} // namespace codex

#endif // CODEX_MEMORY_BOX_H
