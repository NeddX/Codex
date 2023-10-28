#include "Application.h"
#include "../Events/KeyEvent.h"
#include "../Events/MouseEvent.h"
#include "Exception.h"
#include "Input.h"

namespace codex {
    Application* Application::m_Instance = nullptr;

    Application::Application(const ApplicationProperties& args) : m_Properties(args)
    {
        try
        {
            m_Instance = this;
            m_Window   = Window::Box(new Window(), [](Window* window) { delete window; });
            m_Window->Init(m_Properties.windowProperties);
            m_Window->SetEventCallback(BindEventDelegate(this, &Application::OnEvent));
            m_ImGuiLayer = new ImGuiLayer();
            PushOverlay(m_ImGuiLayer);
            m_Input = Input::Get();
        }
        catch (const CodexException& ex)
        {
            CX_EXCEPTION_PRINT(ex);
            std::exit(EXIT_FAILURE);
        }
    }

    Application::~Application()
    {
        Input::Destroy();
        m_Instance = nullptr;
    }

    void Application::Run()
    {
        while (m_Running)
        {
            try
            {
                if (!m_Minimized)
                {
                    for (Layer* layer : m_LayerStack)
                        layer->Update(m_DeltaTime);
                }

                if (Input::IsKeyDown(Key::Escape))
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
        EventDispatcher d(e);
        d.Dispatch<KeyDownEvent>(BindEventDelegate(m_Input, &Input::OnKeyDown_Event));
        d.Dispatch<KeyUpEvent>(BindEventDelegate(m_Input, &Input::OnKeyUp_Event));
        d.Dispatch<MouseDownEvent>(BindEventDelegate(m_Input, &Input::OnMouseDown_Event));
        d.Dispatch<MouseUpEvent>(BindEventDelegate(m_Input, &Input::OnMouseUp_Event));
        d.Dispatch<MouseMoveEvent>(BindEventDelegate(m_Input, &Input::OnMouseMove_Event));
        d.Dispatch<MouseScrollEvent>(BindEventDelegate(m_Input, &Input::OnMouseScroll_Event));
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
