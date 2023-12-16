#ifndef CODEX_MEMORY_WEAK_REFERENCE_H
#define CODEX_MEMORY_WEAK_REFERENCE_H

#include <sdafx.h>

namespace codex {
    // Forward declarations.
    template <typename T>
    class Box;
    template <typename T>
    class Shared;

    template <typename T>
    class Ref
    {
        friend class Box<T>;
        friend class Shared<T>;

    private:
        using BaseType = typename std::remove_extent<T>::type;

    private:
        BaseType*                         m_Ptr{};
        std::variant<Box<T>*, Shared<T>*> m_Owner{};

    public:
        Ref(Box<T>& ptr) noexcept : m_Owner(decltype(m_Owner)(&ptr)) { ptr.AppendRef(*this); }
        Ref(Shared<T>& ptr) noexcept : m_Owner(decltype(m_Owner)(&ptr)) { ptr.AppendRef(*this); }
        ~Ref()
        {
            if (m_Owner.index() == 0)
                std::template get<Box<T>*>(m_Owner)->DetachRef(*this);
            else if (m_Owner.index() == 1)
                std::template get<Shared<T>*>(m_Owner)->DetachRef(*this);
            // m_Owner = std::monostate();
        }

        Ref(const Ref<T>& other) = default;
        Ref(Ref<T>&& other)      = default;

    public:
        constexpr operator bool() const noexcept { return m_Ptr; }
        inline BaseType*       operator->() noexcept { return m_Ptr; }
        inline const BaseType* operator->() const noexcept { return m_Ptr; }
        inline BaseType&       operator*() noexcept { return *m_Ptr; }
        inline const BaseType& operator*() const noexcept { return *m_Ptr; }
    };
} // namespace codex

#endif // CODEX_MEMORY_WEAK_REFERENCE_H
