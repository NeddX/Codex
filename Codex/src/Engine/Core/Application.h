#ifndef CODEX_CORE_APPLICATION_H
#define CODEX_CORE_APPLICATION_H

#include <sdafx.h>

#include "CommonDef.h"
#include "Window.h"

namespace codex {
    struct ApplicationCLIArgs
    {
    public:
        int count = 0;
        const char** args = nullptr;

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
        Window::Properties windowProperties;
    };

    class CODEX_EXPORT Application
    {
        friend int main(int argc, char* argv[]);

    private:
        const ApplicationProperties m_Properties;
        std::unique_ptr<Window> m_Window;

    private:
        static Application* m_Instance;

    public:
        Application(const ApplicationProperties& props);
        virtual ~Application();

    public:
        inline Window& GetWindow() const noexcept
        { return *m_Window; }
        inline Application& Get() const noexcept
        { return *m_Instance; }

    private:
    };

    Application* CreateApplication(const ApplicationCLIArgs args);
} // namespace codex

#endif // CODEX_CORE_APPLICATION_H
