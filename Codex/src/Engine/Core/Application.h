#ifndef CODEX_CORE_APPLICATION_H
#define CODEX_CORE_APPLICATION_H

#include <sdafx.h>

#include "CommonDef.h"
#include "Window.h"

int main(int argc, char** argv);

namespace codex {
    struct ApplicationCLIArgs
    {
    public:
        int count         = 0;
        char** args = nullptr;

    public:
        const char* operator[](const usize index) const
        {
            CX_ASSERT(index > count, "Index out of bounds.");
            return args[index];
        }
    };

    struct ApplicationProperties
    {
        std::string name = "Codex Application";
        std::string cwd;
        ApplicationCLIArgs args;
        WindowProperties windowProperties;
    };

    class Application
    {
        friend int main(int argc, char* argv[]);
        friend Application* CreateApplication(const ApplicationCLIArgs args);

    protected:
        const ApplicationProperties m_Properties;
        Window::Box  m_Window = nullptr;
        bool m_Running = true;

    private:
        static Application* m_Instance;

    public:
        Application(const ApplicationProperties& props);
        virtual ~Application();

    public:
        inline static Window&      GetWindow() noexcept { return *m_Instance->m_Window; }
        inline static Application& Get() noexcept { return *m_Instance; }
        inline static u32          GetFps() noexcept { return m_Instance->m_Window->m_Fps; }

    public:
        void Run();
        void Stop();
    };

    Application* CreateApplication(const ApplicationCLIArgs args);
} // namespace codex

#endif // CODEX_CORE_APPLICATION_H
