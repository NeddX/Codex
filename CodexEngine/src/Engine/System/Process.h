#ifndef CODEX_SYSTEM_PROCESS_H
#define CODEX_SYSTEM_PROCESS_H

#include <sdafx.h>

#include "../Core/Exception.h"
#include "../Memory/Memory.h"

namespace codex::sys {
    CX_CUSTOM_EXCEPTION(ProcessException, "Could not create process.")

    // An enum for the state of the process window.
    enum class WindowState
    {
        Normal,
        Maximized,
        Minimized,
        Hidden
    };

    struct ProcessInfo
    {
        std::string                command;
        std::optional<std::string> cwd             = std::nullopt;
        bool                       redirectStdIn   = false;
        bool                       redirectStdOut  = false;
        bool                       redirectStdErr  = false;
        bool                       systemShell     = true;
        bool                       createWindow    = true;
        bool                       separateConsole = false;
        bool                       shellOpen       = false;
        WindowState                windowState     = WindowState::Normal;
        std::function<void(i32)>   onExit          = nullptr;
    };

    class Process : public mem::SharedManagable<Process>
    {
        friend class mem::Shared<Process>;

    public:
        using ProcessHandle = mem::Shared<Process>;

    protected:
        ProcessInfo m_Info;

    public:
        std::function<void(const char*, usize)> Event_OnOutDataReceived;
        std::function<void(const char*, usize)> Event_OnErrDataReceived;

    public:
        constexpr static auto READ_BUFFER_SIZE = 4096;
        constexpr static auto MAX_ARG_COUNT    = 128;

    protected:
        Process() = default;

    protected:
        explicit Process(ProcessInfo info) noexcept;
        virtual ~Process() = default;

    public:
        static ProcessHandle New(ProcessInfo info) noexcept;

    public:
        virtual void Launch()                              = 0;
        virtual i32  WaitForExit()                         = 0;
        virtual void WriteLine(const std::string_view msg) = 0;
    };
} // namespace codex::sys

#endif // CODEX_SYSTEM_PROCESS_H
