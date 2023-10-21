#include "Application.h"

namespace codex {
    Application* Application::m_Instance = nullptr;

    Application::Application(const ApplicationProperties& args) : m_Properties(args)
    {
        m_Window = std::make_unique<Window>(m_Properties.windowProperties);
    }
}
