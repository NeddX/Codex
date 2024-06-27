#include "Process.h"

#include <sdafx.h>

#ifdef CX_PLATFORM_WINDOWS
#include "../../Platform/Windows/WindowsProcess.h"
#elif defined(CX_PLATFORM_UNIX)
#include "../../Platform/POSIX/POSIXProcess.h"
#endif

namespace codex::sys {
    Process::Process(ProcessInfo info) noexcept : m_Info(std::move(info))
    {
    }

    Process::ProcessHandle Process::New(ProcessInfo info) noexcept
    {
#ifdef CX_PLATFORM_WINDOWS
        return ProcessHandle{ new NTProcess(std::move(info)), [](Process* ptr) { delete ptr; } };
#elif defined(CX_PLATFORM_UNIX)
        return ProcessHandle{ new POSIXProcess(std::move(info)), [](Process* ptr) { delete ptr; } };
#endif
        return nullptr;
    }
} // namespace codex::sys