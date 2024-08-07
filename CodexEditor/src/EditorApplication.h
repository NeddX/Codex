#pragma once

#include <Codex.h>
#include <filesystem>

namespace codex::editor {
    class EditorApplication : public Application
    {
    private:
        static std::filesystem::path m_ApplicationDataPath;
        static std::filesystem::path m_VariableApplicationDataPath;

    public:
        using Application::Application;

    public:
        static std::filesystem::path GetAppDataPath() noexcept;
        static std::filesystem::path GetVarAppDataPath() noexcept;

    public:
        ~EditorApplication() override = default;

    public:
        void OnInit() override;
    };
} // namespace codex::editor
