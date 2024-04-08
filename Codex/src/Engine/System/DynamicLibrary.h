#ifndef CODEX_SYSTEM_DYNAMIC_LIBRARY_H
#define CODEX_SYSTEM_DYNAMIC_LIBRARY_H

#include <sdafx.h>

#include "../Core/Exception.h"

namespace codex {
    CX_CUSTOM_EXCEPTION(DynamicLibraryLoadException, "Failed to load dynamic library.")
    CX_CUSTOM_EXCEPTION(DynamicLibraryInvokeException, "Failed to invoke function")

    class CODEX_API DLib
    {
    private:
#ifdef CX_PLATFORM_WINDOWS
        using DLibInstance = HINSTANCE;
#elif defined(CX_PLATFORM_UNIX)
        using DLibInstance = void*;
#endif

    private:
        DLibInstance m_Handle = (DLibInstance) nullptr;
        std::string  m_FilePath;

    public:
        DLib() = default;
        DLib(const std::string_view filePath);
        DLib(const DLib& other) = delete;
        DLib(DLib&& other) noexcept;
        DLib& operator=(const DLib& other) = delete;
        DLib& operator=(DLib&& other) noexcept;
        ~DLib();

    public:
        void Load(const std::string_view filePath);

    public:
        template <typename Fn, typename... TArgs>
        auto Invoke(const char* func, TArgs&&... args) -> decltype(auto)
        {
#if defined(CX_PLATFORM_UNIX)
            Fn* instance = (Fn*)dlsym(m_Handle, func);
#elif defined(CX_PLATFORM_WINDOWS)
            Fn* instance = (Fn*)GetProcAddress(m_Handle, func);
#endif
            if (!instance)
                cx_throw(DynamicLibraryInvokeException, "Failed to invoke '{}' in '{}'", func, m_FilePath);
            return instance(std::forward<TArgs>(args)...);
        }
    };
} // namespace codex

#endif // CODEX_CORE_DYNAMIC_LIBRARY_H
