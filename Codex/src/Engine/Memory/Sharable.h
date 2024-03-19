#ifndef CODEX_MEMORY_SHARABLE_H
#define CODEX_MEMORY_SHARABLE_H

#include <sdafx.h>

namespace codex::mem {
    // Forward declaration.
    template <typename T>
    class Shared;
    template <typename T>
    class Ref;

    // Base abstract resource manager class using reference counting.
    class ManagableObject
    {
    protected:
        std::atomic<usize> m_Uses  = 1;
        std::atomic<usize> m_Weaks = 1;

    protected:
        constexpr ManagableObject()         = default;
        virtual ~ManagableObject() noexcept = default;

    protected:
        ManagableObject(const ManagableObject&)            = delete;
        ManagableObject& operator=(const ManagableObject&) = delete;

    protected:
        virtual void Drop() noexcept     = 0;
        virtual void DropSelf() noexcept = 0;

    public:
        usize GetUses() const noexcept { return m_Uses.load(); }
        usize GetWeaks() const noexcept { return m_Weaks.load(); }

    public:
        void IncRef() noexcept { ++m_Uses; }
        bool IncRefNz() noexcept
        {
            if (m_Uses != 0)
            {
                ++m_Uses;
                return true;
            }
            return false;
        }
        void IncWRef() noexcept { ++m_Weaks; }
        void DecRef() noexcept
        {
            if (--m_Uses == 0)
            {
                Drop();
                DecWRef();
            }
        }
        void DecWRef() noexcept
        {
            if (--m_Weaks == 0)
            {
                DropSelf();
            }
        }
    };

    template <typename T>
    class ManagedMemory : public ManagableObject
    {
    private:
        T* m_Ptr = nullptr;

    public:
        ManagedMemory(T* const ptr) noexcept : m_Ptr(ptr) {}
        ~ManagedMemory() noexcept override {}

    private:
        void Drop() noexcept override { delete m_Ptr; }
        void DropSelf() noexcept override { delete this; }
    };

    // Resource manager with a custom deleter.
    template <typename T, typename Deleter>
    class ManagedResource : public ManagableObject
    {
    private:
        T*      m_Ptr = nullptr;
        Deleter m_Deleter;

    public:
        ManagedResource(T* const ptr, Deleter deleter) noexcept : m_Ptr(ptr), m_Deleter(deleter) {}
        ~ManagedResource() noexcept override { this->Drop(); }

    private:
        void Drop() noexcept override
        {
            if (m_Ptr)
            {
                m_Deleter(m_Ptr);
                m_Ptr   = nullptr;
                m_Uses  = 1;
                m_Weaks = 1;
            }
        }
        void DropSelf() noexcept override { delete this; }
    };

    // TODO: Resource manager with a custom deleter and an allocator.
    // template <typename T, typename Deleter, typename Allocator>
    // class MemoryControllerAlloc : public ManagableObject

    template <typename T>
    class Sharable
    {
        template <typename U>
        friend class Sharable;

        friend class Shared<T>;

        template <typename U>
        friend class Ref;

    public:
        using BaseType       = std::remove_extent_t<T>;
        using Pointer        = BaseType*;
        using ConstPointer   = const BaseType*;
        using Reference      = BaseType&;
        using ConstReference = const BaseType&;
        using DifferenceType = std::ptrdiff_t;

    protected:
        Pointer          m_Ptr  = nullptr;
        ManagableObject* m_Ctrl = nullptr;

    public:
        constexpr Sharable() noexcept = default;
        // Sharable(const Sharable&)     = delete;
        ~Sharable() = default;

    public:
        // Sharable& operator=(const Sharable&) = delete;

    protected:
        template <typename U>
        void MoveConstructFrom(Sharable<U>&& other) noexcept
        {
            m_Ptr  = other.m_Ptr;
            m_Ctrl = other.m_Ctrl;

            other.m_Ptr  = nullptr;
            other.m_Ctrl = nullptr;
        }
        template <typename U>
        void CopyConstructFrom(const Sharable<U>& other) noexcept
        {
            other.IncRef();

            m_Ptr  = other.m_Ptr;
            m_Ctrl = other.m_Ctrl;
        }
        template <typename U>
        void AliasConstructFrom(const Sharable<U>& other, Pointer aliasPtr) noexcept
        {
            other.IncRef();

            m_Ptr  = aliasPtr;
            m_Ctrl = other.m_Ctrl;
        }
        template <typename U>
        void AliasMoveConstructFrom(Sharable<U>&& other, Pointer aliasPtr) noexcept
        {
            m_Ptr  = aliasPtr;
            m_Ctrl = other.m_Ctrl;

            other.m_Ptr  = nullptr;
            other.m_Ctrl = nullptr;
        }
        template <typename U>
        bool ConstructFromRef(const Ref<U>& other) noexcept
        {
            if (other.m_Ctrl && other.m_Ctrl->IncRefNz())
            {
                m_Ptr  = other.m_Ptr;
                m_Ctrl = other.m_Ctrl;
                return true;
            }
            return false;
        }
        template <typename U>
        void WeaklyConstructFrom(const Sharable<U>& other) noexcept
        {
            if (other.m_Ctrl)
            {
                m_Ptr  = other.m_Ptr;
                m_Ctrl = other.m_Ctrl;
                m_Ctrl->IncWRef();
            }
        }

    protected:
        void IncRef() const noexcept
        {
            if (m_Ctrl)
                m_Ctrl->IncRef();
        }
        void DecRef() const noexcept
        {
            if (m_Ctrl)
                m_Ctrl->DecRef();
        }
        void IncWRef() const noexcept
        {
            if (m_Ctrl)
                m_Ctrl->IncWRef();
        }
        void DecWRef() const noexcept
        {
            if (m_Ctrl)
                m_Ctrl->DecWRef();
        }

    public:
        [[nodiscard]] Pointer Get() const noexcept { return m_Ptr; }
        Sharable<T>&          Swap(Sharable& other) noexcept
        {
            std::swap(m_Ptr, other.m_Ptr);
            std::swap(m_Ctrl, other.m_Ctrl);
            return *this;
        }
    };
} // namespace codex::mem

#endif // CODEX_MEMORY_SHARABLE_H