#include "KeyHandler.h"

namespace codex {
	KeyHandler* KeyHandler::m_Instance = nullptr;

	KeyHandler::KeyHandler()
	{

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
        if (event.action == SDL_PRESSED)
            m_Instance->m_KeysPressed[event.key] = true;
        else if (event.action == SDL_RELEASED)
            m_Instance->m_KeysPressed[event.key] = false;
    }
}
