#include "DynamicLibrary.h"

namespace codex {
    DLib::DLib(const std::string_view filePath) : m_FilePath(filePath)
    {
#if defined(CX_PLATFORM_UNIX)
        m_Handle = dlopen(m_FilePath.c_str(), RTLD_LAZY);
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

            other.m_Handle   = (DLibInstance)0;
            other.m_FilePath = "";
        }
    }

    DLib& DLib::operator=(DLib&& other) noexcept
    {
        if ((uintptr)other.m_Handle)
        {
            m_Handle   = other.m_Handle;
            m_FilePath = other.m_FilePath;

            other.m_Handle   = (DLibInstance)0;
            other.m_FilePath = "";
        }
        return *this;
    }

    DLib::~DLib()
    {
#if defined(CX_PLATFORM_UNIX)
        dlclose(m_Handle);
        m_Handle   = (DLibInstance)0;
        m_FilePath = "";
#endif
    }

    void DLib::Load(const std::string_view filePath)
    {
#if defined(CX_PLATFORM_UNIX)
        m_FilePath = filePath;
        m_Handle   = dlopen(m_FilePath.c_str(), RTLD_LAZY);
        if (!m_Handle)
            cx_throw(DynamicLibraryLoadException, "Failed to load '{}'.", m_FilePath);
#endif
    }
} // namespace codex
