#ifndef CODEX_CORE_APPLICATION_H
#define CODEX_CORE_APPLICATION_H

#include <sdafx.h>

#include "../Events/Event.h"
#include "../ImGui/ImGuiLayer.h"
#include "../Renderer/Renderer.h"
#include "CommonDef.h"
#include "Exception.h"
#include "Input.h"
#include "LayerStack.h"
#include "Window.h"

int main(int argc, char** argv);

namespace codex {
    // Forward declerations.
    class WindowResizeEvent;

    class InvalidPathException : public CodexException
    {
        using CodexException::CodexException;

    public:
        inline const char* default_message() const noexcept override { return "The path supplied is invalid."; }
    };

    struct ApplicationCLIArgs
    {
    public:
        int    count = 0;
        char** args  = nullptr;

    public:
        const char* operator[](const usize index) const
        {
            CX_ASSERT(index > count, "Index out of bounds.");
            return args[index];
        }
    };

    struct ApplicationProperties
    {
        std::string        name = "Codex Application";
        std::string        cwd;
        ApplicationCLIArgs args;
        WindowProperties   windowProperties;
    };

    class Application
    {
        friend int ::main(int argc, char** argv);
        friend Application* CreateApplication(const ApplicationCLIArgs args);

    protected:
        ApplicationProperties                 m_Properties;
        Window::Box                           m_Window    = nullptr;
        bool                                  m_Running   = true;
        bool                                  m_Minimized = false;
        LayerStack                            m_LayerStack;
        std::chrono::system_clock::time_point m_Tp1, m_Tp2;
        f32                                   m_DeltaTime  = 0.0f;
        ImGuiLayer*                           m_ImGuiLayer = nullptr;
        Input*                                m_Input      = nullptr;

    private:
        static Application* m_Instance;

    public:
        Application(const ApplicationProperties& props);
        virtual ~Application();

    public:
        inline static Window&          GetWindow() noexcept { return *m_Instance->m_Window; }
        inline static Application&     Get() noexcept { return *m_Instance; }
        inline static u32              GetFps() noexcept { return (u32)(1.0f / m_Instance->m_DeltaTime); }
        inline static f32              GetDelta() noexcept { return m_Instance->m_DeltaTime; }
        inline static ImGuiLayer*      GetImGuiLayer() noexcept { return m_Instance->m_ImGuiLayer; }
        inline static std::string_view GetCurrentWorkingDirectory() noexcept { return m_Instance->m_Properties.cwd; };
        inline static void             SetCurrentWorkingDirectory(const std::string_view newCwd)
        {
            std::filesystem::path fs_new_cwd = newCwd;
            if (std::filesystem::exists(fs_new_cwd) && std::filesystem::is_directory(fs_new_cwd))
            {
                std::filesystem::current_path(fs_new_cwd);
                m_Instance->m_Properties.cwd = newCwd;
            }
            else
            {
                CX_THROW(InvalidPathException,
                         fmt::format("The path supplied '{}' as the current working directory is invalid.", newCwd));
            }
        }

    public:
        bool OnWindowResize_Event(const WindowResizeEvent& event);

    public:
        void Run();
        void Stop();
        void OnEvent(Event& e);
        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
    };

    Application* CreateApplication(const ApplicationCLIArgs args);
} // namespace codex

#endif // CODEX_CORE_APPLICATION_H
