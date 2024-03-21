#include "POSIXProcess.h"

#include "../../src/Engine/Memory/Memory.h"

namespace codex::sys {
	POSIXProcess::POSIXProcess(ProcessInfo info) noexcept : Process(std::move(info))
	{
				
	}

	void POSIXProcess::Launch() 
	{
		// Create the process.
		m_PID = fork();
		if (m_PID == -1)
			cx_throw(ProcessException, "POSIX: Failed to create process.");
		else if (m_PID == 0) // Child process.
		{
			// Indicate that we're running.
			m_Running = true;

			dup2(m_PipeFd[1], STDOUT_FILENO);
			dup2(m_PipeFd[1], STDOUT_FILENO);

			// Close so that child and parent pipes so that the parent can't write to pipefd[1] and
			// the child doesn't read from pipefd[0].
			close(m_PipeFd[0]);
			close(m_PipeFd[1]);

			// Execute.
			// Use the exec() syscall
			
			// If we reach here then exec() failed.
		}
		else // Parent process.
		{
			if (m_Info.redirectStdOut)
			{
				m_StdOutThread = [this]
				{
					char buffer[Process::READ_BUFFER_SIZE];
					ssize_t bytes_read;
					while (bytes_read = read(m_PipeFd[0], buffer, sizeof(buffer))) > 0)
						this->Event_OnOutDataReceived(buffer, bytes_read);
				};
			}
			if (m_Info.redirectStdErr)
			{
				m_StdErrThread = [this]
				{
					char buffer[Process::READ_BUFFER_SIZE];
					ssize_t bytes_read;
					while (bytes_read = read(m_PipeFd[0], buffer, sizeof(buffer))) > 0)
						this->Event_OnErrDataReceived(buffer, bytes_read);
				};
			}
			
			// Close AGAIN?
			close(m_PipeFd[1]);
		}
	}

	i32 POSIXProcess::WaitForExit()
	{
		if (m_PID != -1)
		{
			// Wait for the process to exit and store the exit code.
			waitpid(m_PID, &m_ExitCode, 0);
			if (WIFEXITED(m_ExitCode))
				return WEXITSTATUS(m_ExitCode);
			else
				cx_throw(ProcessException, "POSIX: Process exited abnormally.");
		}
		cx_throw(InvalidOperationException, "Process hasn't been launched.");
	}
} // namespace codex::sys