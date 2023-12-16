#ifndef CODEX_MEMORY_BOX_H
#define CODEX_MEMORY_BOX_H

#include <sdafx.h>

namespace codex {
    // Forward decleratons.
    template <typename T>
    class Ref;

    template <typename T>
    class Box
    {
        friend class Ref<T>;

    private:
        using BaseType = typename std::remove_extent<T>::type;

    private:
        BaseType*                            m_Ptr = nullptr;
        std::unordered_map<uintptr, Ref<T>*> m_WeakRefs;

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
        ~Box() { Drop(); }

    private:
        inline void Drop()
        {
            if (m_Ptr)
            {
                if (std::is_array_v<T>)
                    delete[] m_Ptr;
                else
                    delete m_Ptr;
                m_Ptr = nullptr;

                for (auto& [k, v] : m_WeakRefs)
                    v->m_Ptr = nullptr;

                m_WeakRefs.clear();
            }
        }
        inline void AppendRef(Ref<T>& ref)
        {
            auto it = m_WeakRefs.find((uintptr)&ref);
            if (it == m_WeakRefs.end())
            {
                m_WeakRefs[(uintptr)&ref] = &ref;
                ref.m_Ptr                 = m_Ptr;
            }
        }
        inline void DetachRef(Ref<T>& ref)
        {
            auto it = m_WeakRefs.find((uintptr)&ref);
            if (it == m_WeakRefs.end())
            {
                m_WeakRefs.erase(it);
                ref.m_Ptr = nullptr;
            }
        }

    public:
        constexpr T*        Get() noexcept { return m_Ptr; }
        constexpr const T*  Get() const noexcept { return m_Ptr; }
        inline Ref<T>       AsRef() noexcept { return *this; }
        inline const Ref<T> AsRef() const noexcept { return *this; }

    public:
        constexpr operator bool() const noexcept { return m_Ptr; }
        inline BaseType*       operator->() noexcept { return m_Ptr; }
        inline const BaseType* operator->() const noexcept { return m_Ptr; }
        inline BaseType&       operator*() noexcept { return *m_Ptr; }
        inline const BaseType& operator*() const noexcept { return *m_Ptr; }
        inline Box<T>&         operator=(const Box<T>& other) noexcept = delete;
        inline Box<T>&         operator=(Box<T>&& other) noexcept
        {
            if (this == &other)
                return *this;

            if (m_Ptr)
                Drop();

            m_Ptr       = other.m_Ptr;
            other.m_Ptr = nullptr;
            return *this;
        }
        inline Box<T>& operator=(const T*&& ptr) noexcept
        {
            if (m_Ptr)
                Drop();

            m_Ptr = ptr;
            ptr   = nullptr;
            return *this;
        }

    public:
        template <typename... TArgs>
        static inline Box<T> New(TArgs&&... args)
        {
            Box<T> box{ new T(std::forward<TArgs>(args)...) };
            return std::move(box);
        }
    };
} // namespace codex

#endif // CODEX_MEMORY_BOX_H
