cmake_minimum_required(VERSION 3.16)

project("Codex")

# For clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

if (WIN32)
	set(CODEX_PLATFORM_WIN32 1)
endif()

# Add sub projects
add_subdirectory("NetNT")
#add_subdirectory("CodexAPI")
add_subdirectory("Codex")
add_subdirectory("TestGame")
