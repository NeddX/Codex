#include "FileSystem.h"

namespace codex::fs {
    std::filesystem::path GetSpecialFolder(const SpecialFolder folder) noexcept
    {
        namespace fs = std::filesystem;

#if defined(CX_PLATFORM_LINUX)
        static const char* home_dir = std::getenv("HOME");
        if (!home_dir)
        {
            struct passwd* pw = getpwuid(getuid());
            if (pw)
                home_dir = pw->pw_dir;
            else
                return "";
        }

        switch (folder)
        {
            using enum SpecialFolder;

            case ApplicationFiles: return "/usr/local/share";
            case ApplicationData: return "/var/local";
            case UserApplicationData: return fs::path(home_dir) / ".config/";
            case Desktop: return fs::path(home_dir) / "/Desktop";
            case Fonts: return "/usr/share/fonts";
            case Temporary: return "/tmp";
            case User: return { home_dir };
            default: return {};
        }
#elif defined(CX_PLATFORM_OSX)

        // bruh...

#elif defined(CX_PLATFORM_WINDOWS)
        static char* home_dir = std::getenv("USERPROFILE");

        switch (folder)
        {
            using enum SpecialFolder;

            case ApplicationFiles: return { std::getenv("ProgramFiles") };
            case ApplicationData: return { std::getenv("APPDATA") };
            case UserApplicationData: return { std::genenv("LOCALAPPDATA") };
            case Desktop: return fs::path(home_dir) / "/Desktop";
            case Fonts: return { "C:/Windows/Fonts" };
            case Temporary: return { std::getenv("temp") };
            case User: return { home_dir };
            default: return {};
        }
#endif
        return "";
    }
} // namespace codex::fs
