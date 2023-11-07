cmake_minimum_required(VERSION 3.16)

project("TestProj")

# For clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

# The target
set(HEADER_FILES "Assets/Scripts/TestScript.h" "Assets/Scripts/TestScript.cpp")
add_library(NBMan SHARED ${HEADER_FILES} ${SOURCE_FILES})

set_property(TARGET NBMan PROPERTY CXX_STANDARD 20)

# ==================================================== #
#   Export macros that describe the current platform   #
# ==================================================== #

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CX_PLATFORM_LINUX 1)
    set(CX_PLATFORM_UNIX 1)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(CX_PLATFORM_OSX 1)
    set(CX_PLATFORM_UNIX 1)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(CX_PLATFORM_WINDOWS 1)
else()
    set(CX_PLATFORM_UNKNOWN 1)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CX_COMPILER_GNUC 1)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
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

# Define macros that describe the current platform.
if(DEFINED CX_PLATFORM_LINUX)
    target_compile_definitions(NBMan PUBLIC CX_PLATFORM_LINUX)
    target_compile_definitions(NBMan PUBLIC CX_PLATFORM_UNIX)
elseif(DEFINED CX_PLATFORM_OSX)
    target_compile_definitions(NBMan PUBLIC CX_PLATFORM_OSX)
    target_compile_definitions(NBMan PUBLIC CX_PLATFORM_UNIX)
elseif(DEFINED CX_PLATFORM_WINDOWS)
    target_compile_definitions(NBMan PUBLIC CX_PLATFORM_WINDOWS)
else()
    target_compile_definitions(NBMan PUBLIC CX_PLATFORM_UNKNOWN)
endif()

if(DEFINED CX_COMPILER_GNUC)
    target_compile_definitions(NBMan PUBLIC CX_COMPILER_GNUC)
elseif(DEFINED CX_COMPILER_CLANG)
    target_compile_definitions(NBMan PUBLIC CX_COMPILER_CLANG)
elseif(DEFINED CX_COMPILER_MSVC)
    target_compile_definitions(NBMan PUBLIC CX_COMPILER_MSVC)
else()
    target_compile_definitions(NBMan PUBLIC CX_COMPILER_UNKNOWN)
endif()

# Determine the processor architecture
if(DEFINED CX_WORD_SIZE_64)
    target_compile_definitions(NBMan PUBLIC CX_WORD_SIZE_64)
else()
    target_compile_definitions(NBMan PUBLIC CX_WORD_SIZE_32)
endif()

# Print the platform information
message(":: Platform OS: ${CMAKE_SYSTEM_NAME}")
message(":: Platform Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(":: CPU word size: ${CMAKE_SIZEOF_VOID_P} bytes")
message("")

add_custom_command(
    TARGET NBMan
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        $<TARGET_FILE_DIR:NBMan>
        "${CMAKE_SOURCE_DIR}/lib/"
)

# Expose Codex's dependencies.
target_include_directories(NBMan PUBLIC Engine/vendor/SDL/include)
target_include_directories(NBMan PUBLIC Engine/vendor/MGL/include)
target_include_directories(NBMan PUBLIC Engine/vendor/json/include)
target_include_directories(NBMan PUBLIC Engine/vendor/imgui)
target_include_directories(NBMan PUBLIC Engine/vendor/glm)
target_include_directories(NBMan PUBLIC Engine/vendor/fmt/include)
target_include_directories(NBMan PUBLIC Engine/vendor/entt)
target_include_directories(NBMan PUBLIC Engine/vendor/glad)
target_include_directories(NBMan PUBLIC Engine/PCH)

target_precompile_headers(NBMan PRIVATE Engine/PCH/sdafx.h)

# Expose the engine API
target_include_directories(NBMan PUBLIC Engine/)
