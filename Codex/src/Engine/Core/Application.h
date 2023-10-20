#ifndef CODEX_CORE_APPLICATION_H
#define CODEX_CORE_APPLICATION_H

#include <sdafx.h>

#include "CommonDef.h"

namespace codex {
    struct ApplicationCLIArgs
    {
    public:
        int count = 0;
        const char** args = nullptr;
        std::unique_ptr<Window> m_Window;

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
    };

    class CODEX_EXPORT Application
    {
    public:
        Application(const ApplicationProperties& props);
        virtual ~Application();

    public:
        inline Window& GetWindow() const noexcept;

    private:
    };

    Application* CreateApplication(const ApplicationCLIArgs args);
} // namespace codex

#endif // CODEX_CORE_APPLICATION_H
