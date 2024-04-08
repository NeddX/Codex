#include "DynamicLibrary.h"

namespace codex {
    DLib::DLib(const std::string_view filePath) : m_FilePath(filePath)
    {
#if defined(CX_PLATFORM_UNIX)
        m_Handle = dlopen(m_FilePath.c_str(), RTLD_LAZY);
        if (!m_Handle)
            cx_throw(DynamicLibraryLoadException, "Failed to load '{}'.", m_FilePath);
#elif defined(CX_PLATFORM_WINDOWS)
        m_Handle = LoadLibraryA(m_FilePath.c_str());
        if (!m_Handle)
            cx_throw(DynamicLibraryLoadException, "Failed to load '{}'.", m_FilePath);
#endif
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

    DLib::~DLib()
    {
#if defined(CX_PLATFORM_UNIX)
        dlclose(m_Handle);
#elif defined(CX_PLATFORM_WINDOWS)
        FreeLibrary(m_Handle);
#endif
        m_Handle   = (DLibInstance) nullptr;
        m_FilePath = "";
    }

    void DLib::Load(const std::string_view filePath)
    {
        m_FilePath = filePath;
#if defined(CX_PLATFORM_UNIX)
        m_Handle = dlopen(m_FilePath.c_str(), RTLD_LAZY);
#elif defined(CX_PLATFORM_WINDOWS)
        m_Handle = LoadLibraryA(m_FilePath.c_str());
#endif
        if (!m_Handle)
            cx_throw(DynamicLibraryLoadException, "Failed to load '{}'.", m_FilePath);
    }
} // namespace codex
