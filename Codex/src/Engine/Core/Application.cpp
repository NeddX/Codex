#include "Application.h"

namespace codex {
    Application* Application::m_Instance = nullptr;

    Application::Application(const ApplicationProperties& args) : m_Properties(args)
    {
        m_Window = Window::Box(new Window(), [](Window* window) { delete window; });
        m_Window->Init(m_Properties.windowProperties);
    }

    Application::~Application()
    {

    }

    void Application::Run()
    {
        // TODO: Do the update stuff here.
        while (true) ;
    }
}
