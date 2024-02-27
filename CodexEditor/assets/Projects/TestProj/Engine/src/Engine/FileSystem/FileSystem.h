#ifndef CODEX_FILE_SYSTEM_H
#define CODEX_FILE_SYSTEM_H

#include <sdafx.h>

namespace codex::fs {
    enum class SpecialFolder
    {
        ApplicationFiles,
        ApplicationData,
        UserApplicationData,
        Desktop,
        Fonts,
        Temporary,
        User
    };

    CODEX_API std::filesystem::path GetSpecialFolder(const SpecialFolder folder) noexcept;
    CODEX_API std::vector<std::filesystem::path> GetAllFilesWithExtensions(
        const std::filesystem::path& directory, const std::initializer_list<std::string_view> extensions) noexcept;
} // namespace codex::fs

#endif // CODEX_FILE_SYSTEM_H
