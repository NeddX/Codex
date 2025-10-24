#include "DynamicLibrary.h"

namespace codex::sys {
    DLib::DLib(std::filesystem::path filePath)
        : m_FilePath(std::move(filePath))
    {
        const auto& file_path_str = m_FilePath.string();
#if defined(CX_PLATFORM_UNIX)
        m_Handle = dlopen(file_path_str.c_str(), RTLD_LAZY);
        // printf("Err: %s\n", dlerror());
#elif defined(CX_PLATFORM_WINDOWS)
        m_Handle = LoadLibraryA(file_path_str.c_str());
#endif
        if (!m_Handle)
            cx_throw(DynamicLibraryLoadException, "Failed to load '{}'.", file_path_str);
    }

    DLib::DLib(DLib&& other) noexcept
    {
        if ((uintptr)other.m_Handle)
        {
            m_Handle   = other.m_Handle;
            m_FilePath = other.m_FilePath;

            other.m_Handle   = (DLibInstance) nullptr;
            other.m_FilePath = "";
        }
    }

    DLib& DLib::operator=(DLib&& other) noexcept
    {
        if ((uintptr)other.m_Handle)
        {
            m_Handle   = other.m_Handle;
            m_FilePath = other.m_FilePath;

            other.m_Handle   = (DLibInstance) nullptr;
            other.m_FilePath = "";
        }
        return *this;
    }

    DLib::~DLib() noexcept
    {
#if defined(CX_PLATFORM_UNIX)
        dlclose(m_Handle);
#elif defined(CX_PLATFORM_WINDOWS)
        FreeLibrary(m_Handle);
#endif

        // FIXME: There's a bug where a DLib instance might have static storage and
        // might get freed after lgx's registry so this line right here will cause a
        // crash.
        lgx::Get("engine").Log(lgx::Info, "~DLib(): {}", m_FilePath.string());

        m_Handle   = (DLibInstance) nullptr;
        m_FilePath = std::filesystem::path{};
    }
} // namespace codex::sys
