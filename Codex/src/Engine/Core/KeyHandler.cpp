#include "KeyHandler.h"

namespace Codex {
	KeyHandler* KeyHandler::m_Instance = nullptr;

	KeyHandler::KeyHandler()
	{
		m_KeysPressed.reset();
	}

	KeyHandler::~KeyHandler()
	{
		
	}

	void KeyHandler::Init() noexcept
	{
		if (!m_Instance)
		{
			m_Instance = new KeyHandler();
			fmt::println("KeyHandler subsystem initialized.");
		}
	}

	void KeyHandler::Destroy() noexcept
	{
		if (m_Instance)
		{
			delete m_Instance;
			m_Instance = nullptr;
			fmt::println("KeyHandler subsystem disposed.");
		}
	}

	void KeyHandler::OnKeyPress_Event(const KeyEvent event)
	{
		if (event.key <= m_Instance->m_KeysPressed.size())
		{
			if (event.action == SDL_PRESSED)
				m_Instance->m_KeysPressed.set(event.key, true);
			else if (event.action == SDL_RELEASED)
				m_Instance->m_KeysPressed.set(event.key, false);
		}
	}
}
