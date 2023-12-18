#ifndef CODEX_MEMORY_SHARED_H
#define CODEX_MEMORY_SHARED_H

#include <sdafx.h>

namespace codex {
    // Forward decleratons.
    template <typename T>
    class Ref;

    template <typename T>
    class Shared
    {
        friend class Ref<T>;

    private:
        using BaseType = typename std::remove_extent<T>::type;

    private:
        BaseType*           m_Ptr      = nullptr;
        std::atomic<usize>* m_RefCount = nullptr;

    private:
        inline static std::unordered_map<uintptr, std::vector<Ref<T>*>> m_WeakRefs{};
        inline static std::mutex                                        m_WeakRefsGuard{};

    public:
        Shared() = default;
        Shared(BaseType*&& rawPtr) noexcept : m_Ptr(rawPtr), m_RefCount(new std::atomic<usize>()) { rawPtr = nullptr; }
        Shared(const Shared<T>& other) noexcept = delete;
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

                    LockGuard lock(m_WeakRefsGuard);
                    for (auto& e : m_WeakRefs[(uintptr)this])
                        e->m_Ptr = nullptr;

                    if (destructing)
                        m_WeakRefs.erase((uintptr)this);
                    else
                        m_WeakRefs[(uintptr)this].clear();
                }
                m_Ptr      = nullptr;
                m_RefCount = nullptr;
            }
        }
        inline void AppendRef(Ref<T>& ref)
        {
            LockGuard lock(m_WeakRefsGuard);
            m_WeakRefs[(uintptr)this].push_back(&ref);
            ref.m_Ptr = m_Ptr;
        }
        inline void DetachRef(Ref<T>& ref)
        {
            LockGuard lock(m_WeakRefsGuard);
            auto&     vec = m_WeakRefs[(uintptr)this];
            vec.erase(std::remove(vec.begin(), vec.end(), &ref), vec.end());
            ref.m_Ptr = nullptr;
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
        inline Shared<T>&      operator=(const Shared<T>& other) noexcept = delete;
        inline Shared<T>&      operator=(Shared<T>&& other) noexcept
        {
            if (this == &other)
                return *this;

            if (m_Ptr)
                Drop();

            m_Ptr       = other.m_Ptr;
            other.m_Ptr = nullptr;
            return *this;
        }
        inline Shared<T>& operator=(const T*&& ptr) noexcept
        {
            if (m_Ptr)
                Drop();

            m_Ptr = ptr;
            ptr   = nullptr;
            return *this;
        }

    public:
        template <typename... TArgs>
        static inline Shared<T> New(TArgs&&... args)
        {
            return std::move(Shared<T>{ new T(std::forward<TArgs>(args)...) });
        }
    };
} // namespace codex

#endif // CODEX_MEMORY_BOX_H
