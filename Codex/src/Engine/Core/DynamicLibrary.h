#ifndef CODEX_CORE_DYNAMIC_LIBRARY_H
#define CODEX_CORE_DYNAMIC_LIBRARY_H

#include <sdafx.h>

#include "Exception.h"

#ifdef CX_PLATFORM_WINDOWS
using DLibInstance = HINSTANCE;
#elif defined(CX_PLATFORM_UNIX)
using DLibInstance = void*;
#endif

namespace codex {
    class DynamicLibraryLoadException : public CodexException
    {
        using CodexException::CodexException;

    public:
        constexpr const char* default_message() const noexcept override { return "Failed to load dynamic library."; }
    };

    class DynamicLibraryInvokeException : public CodexException
    {
        using CodexException::CodexException;

    public:
        constexpr const char* default_message() const noexcept override { return "Failed to invoke function."; }
    };

    class DLib
    {
    private:
        DLibInstance m_Handle = (DLibInstance)0;
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
            if (!instance)
                cx_throw(DynamicLibraryInvokeException, "Failed to invoke '{}' in '{}'", func, m_FilePath);
            return instance(std::forward<TArgs>(args)...);
#elif defined(CX_PLATFORM_WINDOWS)
            Fn* instance = (Fn*)GetProcAddress(m_Handle, func);
            if (!instance)
                cx_throw(DynamicLibraryInvokeException, "Failed to invoke '{}' in '{}'", func, m_FilePath);
            return instance(std::forward<TArgs>(args)...);
#endif
        }
    };
} // namespace codex

#endif // CODEX_CORE_DYNAMIC_LIBRARY_H
