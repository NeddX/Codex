#ifndef CODEX_CORE_IRESOURCE_H
#define CODEX_CORE_IRESOURCE_H

#include <sdafx.h>

namespace Codex
{
	class IResource
	{
	protected:
		size_t m_Id = 0;

	public:
		IResource() = default;
		virtual ~IResource() = default;

	public:
		constexpr size_t inline GetID() const noexcept { return m_Id; }
	};
}

#endif // CODEX_CORE_IRESROUCE_H