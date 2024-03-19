#include "WindowsProcess.h"

namespace codex::sys {
    NTProcess::NTProcess(ProcessInfo info) noexcept : Process(std::move(info))
    {
        ZeroMemory(&m_ProcInfo, sizeof(PROCESS_INFORMATION));
        ZeroMemory(&m_StartInfo, sizeof(STARTUPINFOA));

        m_StartInfo.cb = sizeof(STARTUPINFOA);
    }

    NTProcess::~NTProcess() noexcept
    {
        // Cleanup after we're done.
        CloseHandle(m_ProcInfo.hProcess);
        CloseHandle(m_ProcInfo.hThread);

        // Cleanup the pipes if used any.
        if (m_hStdInWrite)
        {
            CloseHandle(m_hStdInRead);
            CloseHandle(m_hStdInWrite);
        }
        if (m_hStdOutRead)
        {
            if (m_StdOutThread.joinable())
                m_StdOutThread.join();
            CloseHandle(m_hStdOutRead);
            CloseHandle(m_hStdOutWrite);
        }
        if (m_hStdErrRead)
        {
            if (m_StdErrThread.joinable())
                m_StdErrThread.join();
            CloseHandle(m_hStdErrRead);
            CloseHandle(m_hStdErrWrite);
        }
    }

    void NTProcess::CreateChildProcess()
    {
        // Set our handles.
        m_StartInfo.hStdInput  = m_hStdInWrite;
        m_StartInfo.hStdOutput = m_hStdOutWrite;
        m_StartInfo.hStdError  = m_hStdErrRead;
        //m_StartInfo.dwFlags |= CREATE_NEW_CONSOLE;

        // If we have any handle set then tell Windows to use our handles.
        if (m_StartInfo.hStdInput || m_StartInfo.hStdOutput || m_StartInfo.hStdError)
            m_StartInfo.dwFlags |= STARTF_USESTDHANDLES;

        // Try and create the process, If it fails then throw an exception.
        if (!CreateProcessA((m_Info.appName) ? m_Info.appName->c_str() : nullptr, // No app name (shellexecute)
                            (char*)m_Info.command.c_str(),                        // Command line (arguments if app)
                            nullptr,                                              // Process attribs
                            nullptr,                                              // Thread attribs
                            true,                                                 // Inherit handle?
                            0,                                                    // Creation flags
                            nullptr,                                              // Envrionment (use parent's)
                            (m_Info.cwd) ? m_Info.cwd->c_str() : nullptr,         // CWD (use parent's)
                            &m_StartInfo,                                         // Pointer to STARTUPINFOA
                            &m_ProcInfo                                           // Pointer to PROCESS_INFORMATION
                            ))
        {
            cx_throw(InvalidOperationException, "Failed to start process.");
        }
    }

    void NTProcess::Launch()
    {
        // If we want to just run a shell command then create a cmd instance
        // and pass contents of m_Info.command as an argument.
        if (m_Info.systemShell && !m_Info.appName)
            m_Info.command = "cmd /c \"" + m_Info.command + "\"";

        // Security attributes for our anonymous pipes.
        SECURITY_ATTRIBUTES sa_attrs;
        sa_attrs.nLength = sizeof(SECURITY_ATTRIBUTES);
        sa_attrs.bInheritHandle = true;
        sa_attrs.lpSecurityDescriptor = nullptr;

        // If the user wants to redirect stdin, stdout and/or stderr.
        if (m_Info.redirectStdIn)
        {
            if (!CreatePipe(&m_hStdInRead, &m_hStdInWrite, &sa_attrs, 0))
            {
                cx_throw(ProcessException, "Windows: Failed to create pipe for STDIN to redirect to.");
            }
            if (!SetHandleInformation(m_hStdInWrite, HANDLE_FLAG_INHERIT, 0))
            {
                cx_throw(ProcessException, "Windows: Failed to set handle information for STDIN.");
            }
        }
        if (m_Info.redirectStdOut)
        {
            if (!CreatePipe(&m_hStdOutRead, &m_hStdOutWrite, &sa_attrs, 0))
            {
                cx_throw(ProcessException, "Windows: Failed to create pipe for STDOUT to redirect to.");
            }
            if (!SetHandleInformation(m_hStdOutRead, HANDLE_FLAG_INHERIT, 0))
            {
                cx_throw(ProcessException, "Windows: Failed to set handle information for STDOUT.");
            }
        }
        if (m_Info.redirectStdErr)
        {
            if (!CreatePipe(&m_hStdErrRead, &m_hStdErrWrite, &sa_attrs, 0))
            {
                cx_throw(ProcessException, "Windows: Failed to create pipe for STDERR to redirect to.");
            }
            if (!SetHandleInformation(m_hStdErrRead, HANDLE_FLAG_INHERIT, 0))
            {
                cx_throw(ProcessException, "Windows: Failed to set handle information for STDERR.");
            }
        }

        // Create the actual process.
        CreateChildProcess();

        // Indicate that our process is running.
        m_Running = true;

        // A non-blocking thread that will wait for the process to finish, get the exit code and indicate that the
        // process has stopped.
        // Since the thread is detached meaning that the owning shared pointer might get out of scope
        // before our thread finishes, it will capture the instance of that shared pointer thus becoming
        // an owner using NewSharedFromThis() (because Process inherits from mem::SharedManagable).
        std::thread(
            [dummy = NewSharedFromThis().As<NTProcess>(), this]() mutable
            {
                // Create the threads responsible for redirecting stdout and stderr if
                // the user wants to redirect.
                if (Event_OnOutDataReceived)
                {
                    m_StdOutThread = std::thread(
                        [this]
                        {
                            char  buffer[Process::READ_BUFFER_SIZE];
                            DWORD bytes_read;
                            while (ReadFile(m_hStdOutRead, buffer, sizeof(buffer), &bytes_read, nullptr) &&
                                   bytes_read != 0)
                                Event_OnOutDataReceived(buffer, bytes_read);
                        });
                }
                if (Event_OnErrDataReceived)
                {
                    m_StdErrThread = std::thread(
                        [this]
                        {
                            char  buffer[Process::READ_BUFFER_SIZE];
                            DWORD bytes_read;
                            while (ReadFile(m_hStdErrRead, buffer, sizeof(buffer), &bytes_read, nullptr) &&
                                   bytes_read != 0)
                                Event_OnErrDataReceived(buffer, bytes_read);
                        });
                }

                // Wait for our process to finish.
                WaitForSingleObject(m_ProcInfo.hProcess, INFINITE);

                // Try and retrieve the exit code.
                GetExitCodeProcess(m_ProcInfo.hProcess, &m_ExitCode);

                // Indicate that we've stopped.
                m_Running = false;

                // If the user has subscribed to the exit event.
                if (m_Info.onExit)
                    m_Info.onExit(m_ExitCode);
            })
            .detach();
    }

    i32 NTProcess::WaitForExit()
    {
        if (m_ProcInfo.hProcess)
        {
            while (m_Running)
                ;
            return m_ExitCode;
        }
        cx_throw(InvalidOperationException, "Process hasn't been launched.");
    }

    void NTProcess::WriteLine(const std::string_view msg)
    {
        if (m_Info.redirectStdIn)
        {
            DWORD bytes_written;
            if (!WriteFile(m_hStdInWrite, msg.data(), msg.size(), &bytes_written, nullptr))
                cx_throw(ProcessException, "Failed to write to STDIN of a sub-process.");
        }
        else
            cx_throw(InvalidOperationException, "Cannot write to STDIN of a process that has redirection disabled.");
    }
} // namespace codex::sys
