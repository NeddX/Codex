#ifndef CODEX_CORE_EXCEPTION_H
#define CODEX_CORE_EXCEPTION_H

#include <sdafx.h>

#define CX_THROW(ex_type, msg) throw ex_type(msg, __FILE__, CX_PRETTY_FUNCTION, __LINE__)
#define CX_THROW_DEF(ex_type) CX_THROW(ex_type, "")
#define CX_EXCEPTION_PRINT(ex)                                          \
    do                                                                  \
    {                                                                   \
        fmt::println("An exception was caught: {}: {}\n\t{}", typeid(ex).name(), ex.what(), ex.backtrace()); \
    } while (0)

namespace codex {
    class CodexException : public std::exception
    {
    protected:
        const std::string m_Message;
        const char* m_File = nullptr;
        const char* m_Function = nullptr;
        const char* m_ClassName = nullptr;
        const u32 m_Line = 0;

    public:
        CodexException() noexcept = default;
        CodexException(const std::string_view message) noexcept;
        CodexException(const std::string_view message, const char* file, const char* function, const u32 line) noexcept;

    public:
        inline virtual const char* default_message() const noexcept { return "Unknown engine message."; }

    public:
        const char* what() const noexcept override;
        std::string backtrace() const noexcept;
    };
} // namespace codex

#endif // CODEX_CORE_EXCEPTION_H
