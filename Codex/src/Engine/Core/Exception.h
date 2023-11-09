#ifndef CODEX_CORE_EXCEPTION_H
#define CODEX_CORE_EXCEPTION_H

#include <sdafx.h>

#define cx_throw(ex_type, ...)                                                                                    \
    throw ex_type(fmt::format(__VA_ARGS__), __FILE__, CX_PRETTY_FUNCTION, __LINE__)
#define cx_throwd(ex_type) throw ex_type("", __FILE__, CX_PRETTY_FUNCTION, __LINE__)

#define CX_EXCEPTION_PRINT(ex)                                                                                         \
    do                                                                                                                 \
    {                                                                                                                  \
        fmt::println("An exception was caught: {}: {}\n\t{}", ex.TypeNameDemangle(typeid(ex).name()), ex.what(),       \
                     ex.backtrace());                                                                                  \
    } while (0)

namespace codex {

    class CodexException : public std::exception
    {
    protected:
        const std::string m_Message;
        const char*       m_File      = nullptr;
        const char*       m_Function  = nullptr;
        const char*       m_ClassName = nullptr;
        const u32         m_Line      = 0;

    public:
        CodexException() noexcept = default;
        CodexException(const std::string_view message) noexcept;
        CodexException(const std::string_view message, const char* file, const char* function, const u32 line) noexcept;

    public:
        inline virtual const char* default_message() const noexcept { return "Unknown engine message."; }

    public:
        static inline std::string TypeNameDemangle(const char* name)
        {
#if CX_PLATFORM_UNIX
            int                                    status = 0;
            std::unique_ptr<char, void (*)(void*)> res{ abi::__cxa_demangle(name, NULL, NULL, &status), std::free };
            return (status == 0) ? res.get() : name;
#else
            return name;
#endif
        }

    public:
        const char* what() const noexcept override;
        std::string backtrace() const noexcept;
    };
} // namespace codex

#endif // CODEX_CORE_EXCEPTION_H
