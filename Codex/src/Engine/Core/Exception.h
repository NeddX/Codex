#ifndef CODEX_CORE_EXCEPTION_H
#define CODEX_CORE_EXCEPTION_H

#include <sdafx.h>

#define CX_THROW(ex_type, msg) throw ex_type(msg, "cool", "cooler", 123)
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
        const char* m_DefaultMessage = nullptr;
        const char* m_File = nullptr;
        const char* m_Function = nullptr;
        const char* m_ClassName = nullptr;
        const u32 m_Line = 0;

    public:
        CodexException() noexcept = default;
        CodexException(const std::string message) noexcept;
        CodexException(const std::string message, const char* file, const char* function, const u32 line) noexcept;

    public:
        const char* what() const noexcept override;
        std::string backtrace() const noexcept;
    };
} // namespace codex

#endif // CODEX_CORE_EXCEPTION_H
