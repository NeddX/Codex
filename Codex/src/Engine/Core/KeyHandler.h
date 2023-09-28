#ifndef CODEX_CORE_KEY_HANDLER_H
#define CODEX_CORE_KEY_HANDLER_H

#include <sdafx.h>

namespace Codex {
	constexpr auto MAX_KEY_COUNT = 512;

	enum class Key
	{

	};
	// Forward decelerations
	class Window;

	class KeyHandler
	{
#ifdef CX_DEBUG_CUSTOM_ALLOCATORS
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
		static KeyHandler* m_Instance;

	private:
		std::bitset<MAX_KEY_COUNT> m_KeysPressed;

	private:
		KeyHandler();
		~KeyHandler();

	protected:
		struct KeyEvent
		{
			int32_t key;
			uint8_t action, repeat;
		};

	protected:
		static void Init() noexcept;
		static void Destroy() noexcept;

	public:
		static inline bool IsKeyDown(const uint16_t key) noexcept
		{
			if (key <= MAX_KEY_COUNT)
				return m_Instance->m_KeysPressed[key];
			return false;
		}

	protected:
		static void OnKeyPress_Event(const KeyEvent event);
	};
}

#endif // CODEX_CORE_KEY_HANDLER_H
