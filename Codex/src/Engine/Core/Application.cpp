#include "Application.h"
#include "Exception.h"

namespace codex {
    Application* Application::m_Instance = nullptr;

    Application::Application(const ApplicationProperties& args) : m_Properties(args)
    {
        try
        {
            m_Instance = this;
            m_Window   = Window::Box(new Window(), [](Window* window) { delete window; });
            m_Window->Init(m_Properties.windowProperties);
            m_ImGuiLayer = new ImGuiLayer();
            PushOverlay(m_ImGuiLayer);
        }
        catch (const CodexException& ex)
        {
            CX_EXCEPTION_PRINT(ex);
            std::exit(EXIT_FAILURE);
        }
    }

    Application::~Application()
    {
        m_Instance = nullptr;
    }

    void Application::Run()
    {
        while (m_Running)
        {
            try
            {
                m_Window->SetTitle(fmt::format("ms: {} fps: {}", m_DeltaTime, m_Fps).c_str());
                if (!m_Minimized)
                {
                    for (Layer* layer : m_LayerStack)
                        layer->Update(m_DeltaTime);
                }

                if (KeyHandler::IsKeyDown(Key::Escape))
                {
                    Stop();
                    return;
                }

                m_Window->Update(m_DeltaTime);

                m_Tp2       = std::chrono::system_clock::now();
                m_DeltaTime = std::chrono::duration<f32>(m_Tp2 - m_Tp1).count();
                m_Tp1       = m_Tp2;
                m_Fps       = (u32)(1.0f / m_DeltaTime);
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

    void Application::OnEvent(Event& e)
    {
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* overlay)
    {
        m_LayerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }
} // namespace codex
