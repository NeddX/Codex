#ifndef CODEX_MEMORY_WEAK_REFERENCE_H
#define CODEX_MEMORY_WEAK_REFERENCE_H

#include <sdafx.h>

#include "../Core/Exception.h"
#include "Sharable.h"

namespace codex::mem {
    // Forward declarations.
    template <typename T>
    class Shared;

    CX_CUSTOM_EXCEPTION(ExpiredRefException, "Weak reference pointer is expired.")

    template <typename T>
    class Ref : public Sharable<T>
    {
        friend class Shared<T>;

    public:
        constexpr Ref() noexcept = default;
        constexpr Ref(std::nullptr_t) noexcept : Ref() {}

        Ref(const Shared<T>& other) noexcept { this->WeaklyConstructFrom(other); }
        Ref(const Ref<T>& other) noexcept { this->WeaklyConstructFrom(other); }
        Ref(Ref<T>&& other) noexcept { this->MoveConstructFrom(other); }
        template <typename U>
            requires(std::is_convertible_v<U*, T*> || std::is_base_of_v<T, U>)
        Ref(const Shared<U>& other)
        {
            this->WeaklyConstructFrom(other);
        }
        template <typename U>
            requires(std::is_convertible_v<U*, T*> || std::is_base_of_v<T, U>)
        Ref(const Ref<U>& other) noexcept
        {
            this->WeaklyConstructFrom(other);
        }
        template <typename U>
            requires(std::is_convertible_v<U*, T*> || std::is_base_of_v<T, U>)
        Ref(Ref<U>&& other) noexcept
        {
            this->MoveConstructFrom(other);
        }
        ~Ref() noexcept { this->DecWRef(); }

    public:
        constexpr operator bool() const noexcept { return this->m_Ptr != nullptr; }
        Ref<T>&   operator=(const Ref<T>& other)
        {
            Ref<T>{ other }.Swap(*this);
            return *this;
        }
        Ref<T>& operator=(const Shared<T>& other)
        {
            Ref<T>{ other }.Swap(*this);
            return *this;
        }
        template <typename U>
            requires(std::is_convertible_v<U*, T*> || std::is_base_of_v<T, U>)
        Ref<T>& operator=(const Ref<U>& other)
        {
            return this->operator=((const Ref<T>&)other);
        }
        template <typename U>
            requires(std::is_convertible_v<U*, T*> || std::is_base_of_v<T, U>)
        Ref<T>& operator=(const Shared<U>& other)
        {
            return this->operator=((const Shared<T>&)other);
        }

    public:
        inline Shared<T> Lock() const
        {
            Shared<T> ptr;
            if (!ptr.ConstructFromRef(*this))
                cx_throw(ExpiredRefException, "Cannot lock from an expired weak reference pointer.");
            return ptr;
        }
        inline void Reset() noexcept { Ref<T>{}.Swap(*this); }
        inline bool Expired() const noexcept { return !this->m_Ctrl || this->m_Ctrl->GetUses() == 0; }
    };
} // namespace codex::mem

#endif // CODEX_MEMORY_WEAK_REFERENCE_H
