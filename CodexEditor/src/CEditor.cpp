#include "CEditor.h"

#include <filesystem>

#include "EditorLayer.h"
#include "TitleBar.h"

#include <ConsoleMan.h>

namespace codex::editor {
    namespace stdfs = std::filesystem;

    stdfs::path CEditor::m_ApplicationDataPath{};
    stdfs::path CEditor::m_VariableApplicationDataPath{};

    stdfs::path CEditor::GetAppDataPath() noexcept
    {
        return m_ApplicationDataPath;
    }

    stdfs::path CEditor::GetVarAppDataPath() noexcept
    {
        return m_VariableApplicationDataPath;
    }

    void CEditor::OnInit()
    {
#ifdef CX_PLATFORM_UNIX
        m_ApplicationDataPath = stdfs::path(CE_INSTALL_DIR) / stdfs::path("share/CEditor");
#elif defined(CX_PLATFORM_WINDOWS)
        m_ApplicationDataPath = stdfs::path(CE_INSTALL_DIR) / stdfs::path("bin");
#endif
        m_VariableApplicationDataPath =
            stdfs::path(fs::GetSpecialFolder(fs::SpecialFolder::UserApplicationData)) / stdfs::path("CEditor/");

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
       
        //PushLayer(new TitleBar); // Don't need a custom titlebar, for now.
        PushLayer(new EditorLayer);
        PushLayer(new ConsoleMan);
    }
} // namespace codex::editor
