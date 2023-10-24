#ifndef CODEX_CORE_IRESOURCE_H
#define CODEX_CORE_IRESOURCE_H

#include <sdafx.h>

namespace codex {
	class IResource
	{
	protected:
		usize m_Id = 0;

	public:
		IResource() = default;
		virtual ~IResource() = default;

	public:
		constexpr usize inline GetId() const noexcept { return m_Id; }
	};

    template<typename T, typename = typename std::enable_if<std::is_base_of<IResource, T>::value>::type>
    using ResRef = std::shared_ptr<T>;
}

#endif // CODEX_CORE_IRESROUCE_H
