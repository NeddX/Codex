#include "Application.h"
#include "../Events/ApplicationEvent.h"
#include "../Events/KeyEvent.h"
#include "../Events/MouseEvent.h"
#include "../Renderer/DebugDraw.h"
#include "../Scene/Scene.h"
#include "Exception.h"
#include "Input.h"

#include <sdafx.h>

namespace codex {
    Application* Application::m_Instance = nullptr;

    Application::Application(const ApplicationProperties& args) : m_Properties(args)
    {
        try
        {
            std::filesystem::path new_cwd = args.cwd;
            if (std::filesystem::exists(new_cwd) && std::filesystem::is_directory(new_cwd))
            {
                std::filesystem::current_path(new_cwd);
            }
            else
            {
                CX_THROW(InvalidPathException,
                         fmt::format("The path supplied '{}' as the current working directory is invalid.", args.cwd));
            }

            m_Instance = this;
            m_Window   = Window::Box(new Window(), [](Window* window) { delete window; });
            m_Window->Init(m_Properties.windowProperties);
            m_Window->SetEventCallback(BindEventDelegate(this, &Application::OnEvent));

            m_Input = Input::Get();
            DebugDraw::Init();
            Resources::Init();

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
        DebugDraw::Destroy();
        Resources::Destroy();
        Input::Destroy();
        m_Instance = nullptr;
    }

    bool Application::OnWindowResize_Event(const WindowResizeEvent& event)
    {
        i32 x = event.GetWidth(), y = event.GetHeight();
        glViewport(0, 0, x, y);
        if (m_ImGuiLayer)
        {
            auto& io = ImGui::GetIO();
            // m_Window->GetCurrentScene()->GetCamera()->SetWidth(x);
            // m_Window->GetCurrentScene()->GetCamera()->SetHeight(y);
            io.DisplaySize.x = (f32)x;
            io.DisplaySize.y = (f32)y;
        }
        return true;
    }

    void Application::Run()
    {
        while (m_Running)
        {
            try
            {
                m_Window->ProcessEvents();

                if (!m_Minimized)
                {
                    for (Layer* layer : m_LayerStack)
                        layer->Update(m_DeltaTime);

                    if (m_ImGuiLayer)
                    {
                        m_ImGuiLayer->Begin();
                        for (Layer* layer : m_LayerStack)
                            layer->ImGuiRender();
                        m_ImGuiLayer->End();
                    }
                }

                m_Tp2       = std::chrono::system_clock::now();
                m_DeltaTime = std::chrono::duration<f32>(m_Tp2 - m_Tp1).count();
                m_Tp1       = m_Tp2;

                m_Window->SwapBuffers();

                // FIXME: Fix the mouse dragging thing for now...
                Input::EndFrame();
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
        d.Dispatch<WindowResizeEvent>(BindEventDelegate(this, &Application::OnWindowResize_Event));

        for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
        {
            // if (e.handled)
            //     break;
            (*it)->OnEvent(e);
        }
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
