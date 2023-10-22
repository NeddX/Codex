#include "Application.h"
#include "Exception.h"

namespace codex {
    Application* Application::m_Instance = nullptr;

    Application::Application(const ApplicationProperties& args) : m_Properties(args)
    {
        try
        {
            m_Window = Window::Box(new Window(), [](Window* window) { delete window; });
            m_Window->Init(m_Properties.windowProperties);
        }
        catch (const CodexException& ex)
        {
            CX_EXCEPTION_PRINT(ex);
            std::exit(EXIT_FAILURE);
        }
    }

    Application::~Application()
    {

    }

    void Application::Run()
    {
        while (m_Running)
        {
            try
            {
                m_Window->Update();
            }
            catch (const CodexException& ex)
            {
                CX_EXCEPTION_PRINT(ex);
                std::exit(EXIT_FAILURE);
            }
        }
    }

    void Application::Stop()
    {
        m_Running = false;
    }
}
