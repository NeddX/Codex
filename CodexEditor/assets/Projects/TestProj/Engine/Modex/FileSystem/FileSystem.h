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
} // namespace codex::fs

#endif // CODEX_FILE_SYSTEM_H
