#ifndef CODEX_MEMORY_SHARED_H
#define CODEX_MEMORY_SHARED_H

#include <sdafx.h>

#include "../Core/Exception.h"
#include "Sharable.h"

namespace codex::mem {
    // Forward decleratons.
    template <typename T>
    class Ref;
    template <typename T>
    class Shared;

    template <typename T>
    class SharedManagable
    {
        template <typename U>
        friend class Shared;

    public:
        using E_Type = SharedManagable;

    protected:
        Ref<T> m_WeakRef = nullptr;

    protected:
        Shared<T> NewSharedFromThis() noexcept { return m_WeakRef.Lock(); }
        Ref<T>    NewRefFromThis() noexcept { return m_WeakRef; }
    };

    template <class T, class = void>
    struct CanSharedManagable : std::false_type
    {
    }; // detect unambiguous and accessible inheritance from enable_shared_from_this

    template <class T>
    struct CanSharedManagable<T, std::void_t<typename T::E_Type>>
        : std::is_convertible<std::remove_cv_t<T>*, typename T::E_Type*>::type
    {
        // is_convertible is necessary to verify unambiguous inheritance
    };

    template <typename T>
    class Shared : public Sharable<T>

    {
        template <typename U>
        friend class Shared; // We don't ask questions here.

        friend class Ref<T>;

    public:
        using typename Sharable<T>::BaseType;
        using typename Sharable<T>::Pointer;
        using typename Sharable<T>::ConstPointer;
        using typename Sharable<T>::Reference;
        using typename Sharable<T>::ConstReference;
        using typename Sharable<T>::DifferenceType;

    public:
        constexpr Shared() = default;
        constexpr Shared(std::nullptr_t) : Shared() {}

        Shared(const Pointer ptr)
        {
            if constexpr (std::is_array_v<T>)
                InitManager(ptr, new ManagedResource(ptr, std::default_delete<T[]>{}));
            else
                InitManager(ptr, new ManagedMemory(ptr));
        }
        template <typename Deleter>
            requires(std::is_invocable_v<Deleter>)
        Shared(const Pointer ptr, Deleter deleter)
        {
            InitManager(ptr, new ManagedResource(ptr, std::move(deleter)));
        }
        Shared(const Shared<T>& other) { this->CopyConstructFrom(other); }
        Shared(Shared<T>&& other) noexcept { this->MoveConstructFrom(std::move(other)); }
        ~Shared() noexcept { this->DecRef(); }

    public:
        template <typename U>
            requires(std::is_convertible_v<U, T> || std::is_base_of_v<T, U>)
        Shared(U* const ptr) : Shared((const Pointer)ptr)
        {
        }
        template <typename U, typename Deleter>
            requires(std::is_convertible_v<U, T> || std::is_base_of_v<T, U>)
        Shared(U* const ptr, Deleter deleter)
        {
            InitManager(ptr, new ManagedResource(ptr, std::move(deleter)));
        }
        template <typename U>
            requires(std::is_convertible_v<U, T> || std::is_base_of_v<T, U>)
        Shared(const Shared<U>& other)
        {
            this->CopyConstructFrom(other);
        }
        template <typename U>
            requires(std::is_convertible_v<U, T> || std::is_base_of_v<T, U>)
        Shared(Shared<U>&& other) noexcept
        {
            this->MoveConstructFrom(std::move(other));
        }

    private:
        template <typename U>
            requires(std::is_convertible_v<U, T> || std::is_base_of_v<T, U>)
        Shared(U* const ptr, ManagableObject* const cptr) noexcept
        {
            InitManager(ptr, cptr);
        }

    public:
        Shared<T>& operator=(const Shared<T>& other)
        {
            Shared<T>{ other }.Swap(*this);
            return *this;
        }
        Shared<T>& operator=(Shared<T>&& other) noexcept
        {
            Shared<T>{ std::move(other) }.Swap(*this);
            return *this;
        }
        template <typename U>
            requires(std::is_convertible_v<U, T> || std::is_base_of_v<T, U>)
        Shared<T>& operator=(const Shared<U>& other)
        {
            return this->operator=((const Shared<T>&)other);
        }
        template <typename U>
            requires(std::is_convertible_v<U, T> || std::is_base_of_v<T, U>)
        Shared<T>& operator=(Shared<U>&& other) noexcept
        {
            return this->operator=(std::move((Shared<T>&)other));
        }

    private:
        template <typename U>
        void InitManager(U* const ptr, ManagableObject* const ctrl) noexcept
        {
            this->m_Ptr  = ptr;
            this->m_Ctrl = ctrl;

            // This means that T is (directly or indirectly) inheriting from SharedManagable
            //  thus should be able to create a copy (not a new) Shared<T> from within itself.
            // This means we need to assign the m_WeakRef to point to this.
            if constexpr (CanSharedManagable<T>::value)
            {
                if (this->m_Ptr && this->m_Ptr->m_WeakRef.Expired())
                    this->m_Ptr->m_WeakRef = *this;
                // this->m_Ptr->m_WeakRef = Shared<std::remove_cv_t<U>>(*this, (std::remove_cv_t<U>*)this->m_Ptr);
            }
        }

    public:
        inline Ref<T>       AsRef() noexcept { return *this; }
        inline Ref<const T> AsRef() const noexcept { return *this; }

    public:
        constexpr                operator bool() const noexcept { return this->Get() != nullptr; }
        constexpr Pointer        operator->() noexcept { return this->Get(); }
        constexpr ConstPointer   operator->() const noexcept { return this->Get(); }
        constexpr Reference      operator*() noexcept { return *(this->Get()); }
        constexpr ConstReference operator*() const noexcept { return *(this->Get()); }

    public:
        inline Shared<T>& Reset(Pointer&& ptr = nullptr)
        {
            Shared<T>{}.Swap(*this);
            return *this;
        }

    public:
        template <typename U>
        Shared<U> As() const
        {
            auto cast_ptr = Shared<U>{ static_cast<Shared<U>::Pointer>(this->m_Ptr), this->m_Ctrl };
            this->IncRef();
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
