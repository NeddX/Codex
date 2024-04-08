#include "Application.h"
#include "../Events/ApplicationEvent.h"
#include "../Events/KeyEvent.h"
#include "../Events/MouseEvent.h"
#include "../Graphics/DebugDraw.h"
#include "../Scene/Scene.h"
#include "Exception.h"
#include "Input.h"

#include <sdafx.h>

namespace codex {
    namespace fs = std::filesystem;
    using namespace codex::events;
    using namespace codex::imgui;
    using namespace codex::gfx;

    Application* Application::m_Instance = nullptr;

    Application::Application(ApplicationProperties args) : m_Properties(std::move(args))
    {
        try
        {
            fs::path new_cwd = m_Properties.cwd;
            if (fs::exists(new_cwd) && fs::is_directory(new_cwd))
            {
                fs::current_path(new_cwd);
            }
            else
            {
                cx_throw(InvalidPathException, "The path supplied '{}' as the current working directory is invalid.",
                         m_Properties.cwd);
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
            std::cerr << ex << std::endl;
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

    Window& Application::GetWindow() noexcept
    {
        return *m_Instance->m_Window;
    }

    Application& Application::Get() noexcept
    {
        return *m_Instance;
    }

    u32 Application::GetFps() noexcept
    {
        return (u32)(1.0f / m_Instance->m_DeltaTime);
    }

    f32 Application::GetDelta() noexcept
    {
        return m_Instance->m_DeltaTime;
    }

    ImGuiLayer* Application::GetImGuiLayer() noexcept
    {
        return m_Instance->m_ImGuiLayer;
    }

    std::string_view Application::GetCurrentWorkingDirectory() noexcept
    {
        return m_Instance->m_Properties.cwd;
    }

    void Application::SetCurrentWorkingDirectory(const std::string_view newCwd)
    {
        std::filesystem::path fs_new_cwd = newCwd;
        if (std::filesystem::exists(fs_new_cwd) && std::filesystem::is_directory(fs_new_cwd))
        {
            std::filesystem::current_path(fs_new_cwd);
            m_Instance->m_Properties.cwd = newCwd;
        }
        else
        {
            cx_throw(InvalidPathException, "The path supplied '{}' as the current working directory is invalid.",
                     newCwd);
        }
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
                        layer->OnUpdate(m_DeltaTime);

                    if (m_ImGuiLayer)
                    {
                        m_ImGuiLayer->Begin();
                        for (Layer* layer : m_LayerStack)
                            layer->OnImGuiRender();
                        m_ImGuiLayer->End();
                    }
                }

                m_Tp2       = std::chrono::system_clock::now();
                m_DeltaTime = std::chrono::duration<f32>(m_Tp2 - m_Tp1).count();
                m_Tp1       = m_Tp2;

                m_Window->SwapBuffers();

                // FIXME: Fix the mouse dragging thing for now...
                // What?????
                Input::EndFrame();
            }
            catch (const CodexException& ex)
            {
                std::cerr << ex << std::endl;
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
