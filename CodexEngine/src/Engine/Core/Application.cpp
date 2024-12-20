#include "Application.h"
#include "../Events/ApplicationEvent.h"
#include "../Events/KeyEvent.h"
#include "../Events/MouseEvent.h"
#include "../Scene/Scene.h"
#include "Exception.h"
#include "Input.h"

#include <sdafx.h>

namespace codex {
    namespace stdfs = std::filesystem;
    using namespace codex::events;
    using namespace codex::imgui;
    using namespace codex::gfx;

    Application* Application::s_Instance = nullptr;

    Application::Application(ApplicationProperties args)
        : m_Properties(std::move(args))
    {
        // Create the engine logger.
        m_Logger = &lgx::New(
            "engine",
            lgx::Logger::Properties{
                .defaultPrefix = "CodexEngine",
                .defaultStyle  = { .defaultInfoStyle  = fmt::fg(fmt::color::light_sky_blue),
                                   .defaultWarnStyle  = fmt::fg(fmt::color::yellow),
                                   .defaultErrorStyle = fmt::fg(fmt::color::red) | fmt::emphasis::italic,
                                   .defaultFatalStyle = fmt::fg(fmt::color::dark_red) | fmt::emphasis::italic } });

        try
        {
            if (stdfs::exists(m_Properties.cwd) && stdfs::is_directory(m_Properties.cwd))
            {
                stdfs::current_path(m_Properties.cwd);
            }
            else
            {
                cx_throw(InvalidPathException, "The path supplied '{}' as the current working directory is invalid.",
                         m_Properties.cwd.string());
            }

            s_Instance = this;
            m_Window   = Window::Box(new Window(), [](Window* window) { delete window; });
            m_Window->Init(m_Properties.windowProperties);
            m_Window->SetEventCallback(BindEventDelegate(this, &Application::OnEvent));

            m_Input = Input::Get();

            Resources::Init();

            m_ImGuiLayer = new ImGuiLayer();
            PushOverlay(m_ImGuiLayer);
        }
        catch (const CodexException& ex)
        {
            lgx::Get("engine").Log(lgx::Level::Fatal, ex.to_string());
            std::exit(EXIT_FAILURE);
        }
    }

    Application::~Application()
    {
        Resources::Destroy();
        Input::Destroy();
        s_Instance = nullptr;
    }

    auto Application::OnWindowResize_Event(const WindowResizeEvent& event) -> bool
    {
        const auto x = event.GetWidth();
        const auto y = event.GetHeight();
        glViewport(0, 0, x, y);
        if (m_ImGuiLayer)
        {
            auto& io         = ImGui::GetIO();
            io.DisplaySize.x = (f32)x;
            io.DisplaySize.y = (f32)y;
        }
        return true;
    }

    void Application::Run()
    {
        using clock = std::chrono::high_resolution_clock;

        while (m_Running)
        {
            const auto frame_start = clock::now();

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

                m_Window->SwapBuffers();

                // FIXME: Fix the mouse dragging thing for now...
                // What?????
                Input::EndFrame();
            }
            catch (const CodexException& ex)
            {
                lgx::Get("engine").Log(lgx::Level::Fatal, ex.to_string());
                std::exit(EXIT_FAILURE);
            }

            const auto frame_time = std::chrono::duration_cast<std::chrono::milliseconds>(clock::now() - frame_start);

            // Cap the framerate if needed.
            if (m_Properties.windowProperties.frameCap > 0)
            {
                static auto desired_frame_time =
                    std::chrono::milliseconds(1000 / m_Properties.windowProperties.frameCap);

                if (frame_time < desired_frame_time)
                    std::this_thread::sleep_for(desired_frame_time - frame_time);
            }

            m_DeltaTime = std::chrono::duration<f32>(clock::now() - frame_start).count();
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
            if (e.handled)
                break;
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
