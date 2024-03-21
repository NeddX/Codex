#ifndef CODEX_PLATFORM_POSIX_PROCESS_H
#define CODEX_PLATFORM_POSIX_PROCESS_H

#include <sdafx.h>

#include "../../src/Engine/System/Process.h"

namespace codex::sys {
	class POSIXProcess : public Process
	{
		// So that Process::New() can create a mem::Shared<POSIXProcess> with a custom deleter.
		friend class Process;

	private:
		pid_t m_PID;
		i32 m_PipeFd[2];
		i32 m_ExitCode;
		std::thread m_StdOutThread;
		std::thread m_StdErrThread;

	public:
		POSIXProcess(ProcessInfo info) noexcept;
		~POSIXProcess() noexcept override;

	public:
		void Launch() override;
		i32 WaitForExit() override;
		void WriteLine(const std::string_view msg) override;
	};
} // namespace codex::sys

#endif // CODEX_PLATFORM_POSIX_PROCESS_H