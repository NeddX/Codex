#include "CEditor.h"

#include <filesystem>

#include "EditorLayer.h"
#include "TitleBar.h"

namespace codex::editor {
    CODEX_USE_ALL_NAMESPACES()

    std::filesystem::path CEditor::m_ApplicationDataPath{};
    std::filesystem::path CEditor::m_VariableApplicationDataPath{};

    std::filesystem::path CEditor::GetAppDataPath() noexcept
    {
        return m_ApplicationDataPath;
    }

    std::filesystem::path CEditor::GetVarAppDataPath() noexcept
    {
        return m_VariableApplicationDataPath;
    }

    void CEditor::Init()
    {
#ifdef CX_PLATFORM_UNIX
        m_ApplicationDataPath = fs::path(CE_INSTALL_DIR) / fs::path("share/CEditor");
#elif defined(CX_PLATFORM_WINDOWS)
        m_ApplicationDataPath = fs::path(CE_INSTALL_DIR) / fs::path("bin");
#endif
        m_VariableApplicationDataPath =
            fs::path(GetSpecialFolder(SpecialFolder::UserApplicationData)) / fs::path("CEditor/");

        if (!fs::exists(m_VariableApplicationDataPath))
        {
            try
            {
                fs::create_directory(m_VariableApplicationDataPath);
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
    }
} // namespace codex::editor
