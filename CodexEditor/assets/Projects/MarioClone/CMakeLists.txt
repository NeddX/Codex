cmake_minimum_required(VERSION 3.16)

project("MarioClone")

# For LSPs.
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

# The target
file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Assets/*.h")
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Assets/*.cpp")
file(GLOB_RECURSE CODGEN_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/int/*.h")
file(GLOB_RECURSE CODGEN_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/int/*.cpp")
add_library(NBMan SHARED ${HEADER_FILES} ${SOURCE_FILES} ${CODGEN_HEADERS}
                         ${CODGEN_SOURCES})

if(DEFINED CX_PLATFORM_WINDOWS)

endif()

set_property(TARGET NBMan PROPERTY CXX_STANDARD 20)
set(CMAKE_SHARED_LIBRARY_PREFIX "lib")
set(CMAKE_STATIC_LIBRARY_PREFIX "lib")
set_target_properties(NBMan PROPERTIES OUTPUT_NAME "NBMan")
set_target_properties(NBMan PROPERTIES SUFFIX ".dll")

# ==================================================== # Export macros that
# describe the current platform   #
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

# Define macros that describe the current platform.
if(DEFINED CX_PLATFORM_LINUX)
  target_compile_definitions(NBMan PUBLIC CX_PLATFORM_LINUX)
  target_compile_definitions(NBMan PUBLIC CX_PLATFORM_UNIX)
elseif(DEFINED CX_PLATFORM_OSX)
  target_compile_definitions(NBMan PUBLIC CX_PLATFORM_OSX)
  target_compile_definitions(NBMan PUBLIC CX_PLATFORM_UNIX)
  target_link_options(NBMan PUBLIC -dynamiclib -flat_namespace -undefined dynamic_lookup)
elseif(DEFINED CX_PLATFORM_WINDOWS)
  target_compile_definitions(NBMan PUBLIC CX_PLATFORM_WINDOWS)
  target_link_libraries(NBMan PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/lib/CodexEngine.lib
	${CMAKE_CURRENT_SOURCE_DIR}/lib/MGL.lib
	${CMAKE_CURRENT_SOURCE_DIR}/lib/fmtd.lib )
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
  COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_SOURCE_DIR}/lib/"
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:NBMan>
          "${CMAKE_SOURCE_DIR}/lib/")

# Expose Codex's dependencies.
# TODO: Automate this please cause we have to add an entry here everytime we add a new library.
target_include_directories(NBMan PUBLIC Engine/vendor/SDL/include)
target_include_directories(NBMan PUBLIC Engine/vendor/MGL/include)
target_include_directories(NBMan PUBLIC Engine/vendor/json/include)
target_include_directories(NBMan PUBLIC Engine/vendor/imgui)
target_include_directories(NBMan PUBLIC Engine/vendor/glm)
target_include_directories(NBMan PUBLIC Engine/vendor/fmt/include)
target_include_directories(NBMan PUBLIC Engine/vendor/entt)
target_include_directories(NBMan PUBLIC Engine/vendor/glad)
target_include_directories(NBMan PUBLIC Engine/vendor/box2d/include)
target_include_directories(NBMan PUBLIC Engine/src/PCH)

# For ease of use.
target_include_directories(NBMan PUBLIC Engine/src)

# Reflector
target_include_directories(NBMan PRIVATE int/)

target_precompile_headers(NBMan PRIVATE Engine/src/PCH/sdafx.h)

# Expose the engine API
target_include_directories(NBMan PUBLIC Engine/include)

# If we're not on VS, then copy compile_commands.json for LSPs like clangd and
# ccls.
if(NOT CMAKE_GENERATOR MATCHES "Visual Studio")
  add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/compile_commands.json
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/compile_commands.json
            ${CMAKE_SOURCE_DIR}/compile_commands.json
    DEPENDS ${CMAKE_BINARY_DIR}/compile_commands.json
    COMMENT "Copying compile_commands.json...")

  add_custom_target(copy_compile_commands ALL
                    DEPENDS ${CMAKE_SOURCE_DIR}/compile_commands.json)
endif()
