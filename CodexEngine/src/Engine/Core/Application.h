#ifndef CODEX_CORE_APPLICATION_H
#define CODEX_CORE_APPLICATION_H

#include <sdafx.h>

#include "../Events/Event.h"
#include "../Graphics/Renderer.h"
#include "../ImGui/ImGuiLayer.h"
#include "CommonDef.h"
#include "Exception.h"
#include "LayerStack.h"
#include "Window.h"

int main(int argc, char** argv);

namespace codex {
    // Forward declarations.
    class Input;
    namespace events {
        class WindowResizeEvent;
    } // namespace events

    CX_CUSTOM_EXCEPTION(InvalidPathException, "The path supplied is invalid.");

    struct ApplicationCLIArgs
    {
    public:
        int    count = 0;
        char** args  = nullptr;

    public:
        const char* operator[](const usize index) const noexcept
        {
            CX_ASSERT(index > count, "Index out of bounds.");
            return args[index];
        }
    };

    struct ApplicationProperties
    {
        std::string           name = "Codex Application";
        std::filesystem::path cwd;
        ApplicationCLIArgs    args;
        WindowProperties      windowProperties;
    };

    class CODEX_API Application
    {
        friend int ::main(int argc, char** argv);
        friend Application* CreateApplication(ApplicationCLIArgs args);

    protected:
        ApplicationProperties m_Properties;
        Window::Box           m_Window    = nullptr;
        bool                  m_Running   = true;
        bool                  m_Minimized = false;
        LayerStack            m_LayerStack;
        f32                   m_DeltaTime  = 0.0f;
        imgui::ImGuiLayer*    m_ImGuiLayer = nullptr;
        Input*                m_Input      = nullptr;

    private:
        static Application* s_Instance;

    public:
        Application(ApplicationProperties props);
        Application(const Application& other) = delete;
        virtual ~Application();

    public:
        static Window&               GetWindow() noexcept;
        static Application&          Get() noexcept;
        static u32                   GetFps() noexcept;
        static u32                   GetFrameCap() noexcept;
        static f32                   GetDelta() noexcept;
        static imgui::ImGuiLayer*    GetImGuiLayer() noexcept;
        static std::filesystem::path GetCurrentWorkingDirectory() noexcept;
        static void                  SetCurrentWorkingDirectory(const std::filesystem::path& newCwd);

    public:
        bool OnWindowResize_Event(const events::WindowResizeEvent& event);

    public:
        virtual void OnInit() {};

    public:
        void Run();
        void Stop();
        void OnEvent(events::Event& e);
        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
    };

    Application* CreateApplication(const ApplicationCLIArgs args);
} // namespace codex

#endif // CODEX_CORE_APPLICATION_H
