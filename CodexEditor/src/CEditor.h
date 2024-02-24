#pragma once

#include <Codex.h>
#include <filesystem>

#define CODEX_USE_ALL_NAMESPACES()                                                                                     \
    namespace fs = std::filesystem;                                                                                    \
    using namespace codex;                                                                                             \
    using namespace codex::fs;                                                                                         \
    using namespace codex::mem;                                                                                        \
    using namespace codex::imgui;                                                                                      \
    using namespace codex::events;                                                                                     \
    using namespace codex::reflect;                                                                                    \
    using namespace codex::graphics;

namespace codex::editor {
    class CEditor : public Application
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
        ~CEditor() override = default;

    public:
        void Init() override;
    };
} // namespace codex::editor
