#include "MouseHandler.h"

namespace Codex {
	MouseHandler* MouseHandler::m_Instance = nullptr;

	MouseHandler::MouseHandler()
	{
		m_ScrollX	= m_ScrollY		= 0;
		m_PosX		= m_PosY		= 0;
		m_LastPosX	= m_LastPosY	= 0;
	}
	
	MouseHandler::~MouseHandler()
	{
		
	}

	void MouseHandler::Init() noexcept
	{
		if (!m_Instance)
		{
			m_Instance = new MouseHandler();
			fmt::println("MouseHandler subsystem initialized.");
		}
	}

	void MouseHandler::Destroy() noexcept
	{
		if (m_Instance)
		{
			delete m_Instance;
			m_Instance = nullptr;
			fmt::println("MouseHandler subsystem disposed.");
		}
	}

	void MouseHandler::EndFrame() noexcept
	{
		m_Instance->m_ScrollX = 0;
		m_Instance->m_ScrollY = 0;
		m_Instance->m_LastPosX = m_Instance->m_PosX;
		m_Instance->m_LastPosY = m_Instance->m_PosY;
	}

	void MouseHandler::OnMouseMove_Event(const MouseEvent event) noexcept
	{
		m_Instance->m_LastPosX = m_Instance->m_PosX;
		m_Instance->m_LastPosY = m_Instance->m_PosY;
		m_Instance->m_PosX = event.x;
		m_Instance->m_PosY = event.y;
		m_Instance->m_Dragging = m_Instance->m_ButtonsPressed.any();
	}

	void MouseHandler::OnMouseButton_Event(const MouseEvent event) noexcept
	{
		if (event.button <= 2)
		{
			if (event.action == SDL_PRESSED)
			{
				m_Instance->m_ButtonsPressed[event.button] = true;
			}
			else if (event.action == SDL_RELEASED)
			{
				m_Instance->m_ButtonsPressed[event.button] = false;
				m_Instance->m_Dragging = false;
			}
		}
	}

	void MouseHandler::OnMouseScroll_Event(const MouseEvent event) noexcept
	{
		m_Instance->m_ScrollX = event.scrollX;
		m_Instance->m_ScrollY = event.scrollY;
		// TODO: Handle scroll direction...
	}
}
