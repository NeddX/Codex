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
}

#endif // CODEX_CORE_IRESROUCE_H