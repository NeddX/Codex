#ifndef CODEX_CORE_MOUSE_HANDLER_H
#define CODEX_CORE_MOUSE_HANDLER_H

#include <sdafx.h>

#include "Memory.h"

namespace Codex
{
	// Forward decelerations
	class Window;

	class MouseHandler : public IDisposable
	{
#ifdef CDX_DEBUG_CUSTOM_ALLOCATORS
	public:
		void* operator new(size_t size)
		{
			void* ptr = std::malloc(size);
			fmt::println("[Memory] :: Allocated memory.\n\tFile: {}\n\tLine: {}\n\tSize: {}\n\tAddress: {}",
				__FILE__, __LINE__, size, ptr);
			return ptr;
		}
		void operator delete(void* ptr)
		{
			fmt::println("[Memory] :: Deallocated memory.\n\tFile: {}\n\tLine: {}\n\tAddress: {}", __FILE__, __LINE__, ptr);
			std::free(ptr);
		}
#endif
		friend class Window;

	private:
		static MouseHandler* m_Instance;

	private:
		int m_ScrollX, m_ScrollY;
		int m_PosX, m_PosY, m_LastPosX, m_LastPosY;
		std::bitset<3> m_ButtonsPressed;
		bool m_Dragging, m_Disposed;

	private:
		MouseHandler();
		~MouseHandler();
	
	public:
		void Dispose() override;

	protected:
		struct MouseEvent
		{
			int			x, y, scrollX, scrollY;
			uint8_t		action, button, clicks;
			uint32_t	scrollDir;
		};

	public:
		static inline int GetMouseX() noexcept
		{
			return m_Instance->m_PosX;
		}
		static inline int GetMouseY() noexcept
		{
			return m_Instance->m_PosY;
		}
		static inline int GetMouseDeltaX() noexcept
		{
			return m_Instance->m_LastPosX - m_Instance->m_PosX;
		}
		static inline int GetMouseDeltaY() noexcept
		{
			return m_Instance->m_LastPosY - m_Instance->m_PosY;
		}
		static inline int GetScrollX() noexcept
		{
			return m_Instance->m_ScrollX;
		}
		static inline int GetScrollY() noexcept
		{
			return m_Instance->m_ScrollY;
		}
		static inline bool GetIsDragging() noexcept
		{
			return m_Instance->m_Dragging;	
		}
		static inline bool IsMouseDown(uint8_t mouseButton) noexcept
		{
			if (mouseButton <= 3) return m_Instance->m_ButtonsPressed[mouseButton];
			return false;
		}

	protected:
		static void Init() noexcept;
		static void Deinit() noexcept;
		
	protected:
		void EndFrame() noexcept;	

	protected:
		static void OnMouseMove_Event(const MouseEvent event) noexcept;
		static void OnMouseButton_Event(const MouseEvent event) noexcept;
		static void OnMouseScroll_Event(const MouseEvent event) noexcept;
	};
}

#endif // CODEX_CORE_MOUSE_HANDLER_H
