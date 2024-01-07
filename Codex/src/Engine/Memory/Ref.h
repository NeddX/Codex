#ifndef CODEX_MEMORY_WEAK_REFERENCE_H
#define CODEX_MEMORY_WEAK_REFERENCE_H

#include <sdafx.h>

#include "../Core/Exception.h"

namespace codex {
    // Forward declarations.
    template <typename T>
    class Shared;

    template <typename T>
    class Ref
    {
        friend class Shared<T>;

    private:
        using BaseType = typename std::remove_extent<T>::type;

    private:
        const Shared<T>* m_Owner{};

    public:
        Ref() = default;
        Ref(const Shared<T>& shared) noexcept : m_Owner(&shared) {}

    public:
        constexpr operator bool() const noexcept { return *m_Owner->m_Ptr; }
        /*constexpr BaseType*  operator->() noexcept { return *m_Ptr; }
        constexpr const BaseType* operator->() const noexcept { return *m_Ptr; }
        constexpr BaseType&       operator*() noexcept { return **m_Ptr; }
        constexpr const BaseType& operator*() const noexcept { return **m_Ptr; }
        constexpr BaseType*       Get() noexcept { return *m_Ptr; }
        constexpr const BaseType* Get() const noexcept { return *m_Ptr; }
        */

    public:
        constexpr Shared<T> Lock() const noexcept { return *m_Owner; }
    };
} // namespace codex

#endif // CODEX_MEMORY_WEAK_REFERENCE_H
