#include "KeyHandler.h"

namespace Codex
{
	KeyHandler* KeyHandler::m_Instance = nullptr;
	
	KeyHandler::KeyHandler()
	{
		m_KeysPressed.reset();
	}

	KeyHandler::~KeyHandler()
	{
		
	}

	void KeyHandler::Dispose()
	{
		m_Disposed = true;
		delete this;
	}

	void KeyHandler::Init() noexcept
	{
		if (!m_Instance)
			m_Instance = new KeyHandler();
	}

	void KeyHandler::Deinit() noexcept
	{
		if (!m_Instance->m_Disposed)
			m_Instance->Dispose();
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
