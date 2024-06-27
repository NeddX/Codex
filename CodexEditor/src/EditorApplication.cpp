#include "EditorApplication.h"

#include <filesystem>

#include "Editor.h"
#include "TitleBar.h"

#include <ConsoleMan.h>

namespace codex::editor {
    namespace stdfs = std::filesystem;

    stdfs::path EditorApplication::m_ApplicationDataPath{};
    stdfs::path EditorApplication::m_VariableApplicationDataPath{};

    stdfs::path EditorApplication::GetAppDataPath() noexcept
    {
        return m_ApplicationDataPath;
    }

    stdfs::path EditorApplication::GetVarAppDataPath() noexcept
    {
        return m_VariableApplicationDataPath;
    }

    void EditorApplication::OnInit()
    {
#ifdef CX_PLATFORM_UNIX
        m_ApplicationDataPath = stdfs::path(CE_INSTALL_DIR) / stdfs::path("share/CEditor");
#elif defined(CX_PLATFORM_WINDOWS)
        m_ApplicationDataPath = stdfs::path(CE_INSTALL_DIR) / stdfs::path("bin");
#endif
        m_VariableApplicationDataPath =
            stdfs::path(fs::GetSpecialFolder(fs::SpecialFolder::UserApplicationData)) / stdfs::path("CEditor/");

        stdfs::current_path(m_ApplicationDataPath);

        if (!stdfs::exists(m_VariableApplicationDataPath))
        {
            try
            {
                stdfs::create_directory(m_VariableApplicationDataPath);
            }
            catch (const std::exception& ex)
            {
                fmt::println("[Warning] :: Failed to create application data folder!");
            }
        }

        fmt::println("[Info] :: Application data path: '{}'", m_ApplicationDataPath.string());
        fmt::println("[Info] :: Variable application data path: '{}'", m_VariableApplicationDataPath.string());

        // PushLayer(new TitleBar); // Don't need a custom titlebar, for now.
        PushLayer(new Editor);
        PushLayer(new ConsoleMan);
    }
} // namespace codex::editor
