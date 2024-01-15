cmake_minimum_required(VERSION 3.16)

project("Codex")

# For clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

# ==================================================== # Export macros that
# describe the current platform   #
# ==================================================== #

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(CX_PLATFORM_LINUX 1)
  set(CX_PLATFORM_UNIX 1)
  set(CX_VARIABLE_PATH "${CMAKE_INSTALL_PREFIX}/share")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  set(CX_PLATFORM_OSX 1)
  set(CX_PLATFORM_UNIX 1)
  set(CX_VARIABLE_PATH "${CMAKE_INSTALL_PREFIX}/share")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(CX_PLATFORM_WINDOWS 1)
  set(CX_APP_DATA_PATH "${CMAKE_INSTALL_PREFIX}")
  set(CX_APP_DATA_VARIABLE_PATH "${CMAKE_INSTALL_PREFIX}/share")
else()
  set(CX_PLATFORM_UNKNOWN 1)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(CX_COMPILER_GNUC 1)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL
                                                 "AppleClang")
  set(CX_COMPILER_CLANG 1)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  set(CX_COMPILER_MSVC 1)
else()
  set(CX_COMPILER_UNKNOWN 1)
endif()

# Determine the processor architecture
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(CX_WORD_SIZE_64 1)
else()
  set(CX_WORD_SIZE_32 1)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CX_BUILD_TYPE_DEBUG 1)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(CX_BUILD_TYPE_RELEASE 1)
elseif(CMAKE_BUILD_TYPE STREQUAL "Shipping")
  set(CX_BUILD_TYPE_SHIPPING 1)
endif()

# Add sub projects add_subdirectory("NetNT") add_subdirectory("CodexAPI")
add_subdirectory("Codex")
# add_subdirectory("TestGame")
add_subdirectory("CodexEditor")
