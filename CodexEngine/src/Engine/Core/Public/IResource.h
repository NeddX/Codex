#ifndef CODEX_CORE_IRESOURCE_H
#define CODEX_CORE_IRESOURCE_H

#include <sdafx.h>

#include <Engine/Memory/Public/Memory.h>

namespace codex {
    class IResource
    {
    protected:
        usize m_Id = 0;

    public:
        IResource()          = default;
        virtual ~IResource() = default;

    public:
        constexpr usize GetId() const noexcept { return m_Id; }
    };

    template <typename T>
        requires(std::is_base_of_v<IResource, T>)
    using ResRef = mem::Shared<T>;
} // namespace codex

#endif // CODEX_CORE_IRESROUCE_H
