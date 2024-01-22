# Fetch all the source and header files and the then add them automatically
file(GLOB_RECURSE ENGINE_SOURCES "src/Engine/*.cpp")
file(GLOB_RECURSE ENGINE_HEADERS "src/Engine/*.h")

# Export all symbols on Windows (NOTE TO SELF: Do not uncomment).
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library(CodexEngine SHARED ${ENGINE_HEADERS} ${ENGINE_SOURCES})
add_library(CodexEngine-static STATIC ${ENGINE_HEADERS} ${ENGINE_SOURCES})

# Set the C++ Standard to 20 for this target.
set_property(TARGET CodexEngine PROPERTY CXX_STANDARD 20)
set_property(TARGET CodexEngine-static PROPERTY CXX_STANDARD 20)

# Tell the guy to export the symbols. This is only relevant for Windows really.
target_compile_definitions(CodexEngine PRIVATE CX_BUILD_SHARED)
target_compile_definitions(CodexEngine-static PUBLIC CX_BUILD_STATIC)

# Position independent code for shared libraries.
if(DEFINED CX_PLATFORM_UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

target_compile_definitions(CodexEngine PRIVATE CX_BUILD_LIB)

# Define macros that describe the current platform.
if(DEFINED CX_PLATFORM_LINUX)
  target_compile_definitions(CodexEngine PUBLIC CX_PLATFORM_LINUX)
  target_compile_definitions(CodexEngine PUBLIC CX_PLATFORM_UNIX)
  target_compile_definitions(CodexEngine-static PUBLIC CX_PLATFORM_LINUX)
  target_compile_definitions(CodexEngine-static PUBLIC CX_PLATFORM_UNIX)
elseif(DEFINED CX_PLATFORM_OSX)
  target_compile_definitions(CodexEngine PUBLIC CX_PLATFORM_OSX)
  target_compile_definitions(CodexEngine PUBLIC CX_PLATFORM_UNIX)
  target_compile_definitions(CodexEngine-static PUBLIC CX_PLATFORM_OSX)
  target_compile_definitions(CodexEngine-static PUBLIC CX_PLATFORM_UNIX)
elseif(DEFINED CX_PLATFORM_WINDOWS)
  target_compile_definitions(CodexEngine PUBLIC CX_PLATFORM_WINDOWS)
  target_compile_definitions(CodexEngine-static PUBLIC CX_PLATFORM_WINDOWS)
else()
  target_compile_definitions(CodexEngine PUBLIC CX_PLATFORM_UNKNOWN)
  target_compile_definitions(CodexEngine-static PUBLIC CX_PLATFORM_UNKNOWN)
endif()

if(DEFINED CX_COMPILER_GNUC)
  target_compile_definitions(CodexEngine PUBLIC CX_COMPILER_GNUC)
  target_compile_definitions(CodexEngine-static PUBLIC CX_COMPILER_GNUC)
elseif(DEFINED CX_COMPILER_CLANG)
  target_compile_definitions(CodexEngine PUBLIC CX_COMPILER_CLANG)
  target_compile_definitions(CodexEngine-static PUBLIC CX_COMPILER_CLANG)
elseif(DEFINED CX_COMPILER_MSVC)
  target_compile_definitions(CodexEngine PUBLIC CX_COMPILER_MSVC)
  target_compile_definitions(CodexEngine-static PUBLIC CX_COMPILER_MSVC)
else()
  target_compile_definitions(CodexEngine PUBLIC CX_COMPILER_UNKNOWN)
  target_compile_definitions(CodexEngine-static PUBLIC CX_COMPILER_UNKNOWN)
endif()

# Determine the processor architecture
if(DEFINED CX_WORD_SIZE_64)
  target_compile_definitions(CodexEngine PUBLIC CX_WORD_SIZE_64)
  target_compile_definitions(CodexEngine-static PUBLIC CX_WORD_SIZE_64)
else()
  target_compile_definitions(CodexEngine PUBLIC CX_WORD_SIZE_32)
  target_compile_definitions(CodexEngine-static PUBLIC CX_WORD_SIZE_32)
endif()

if(DEFINED CX_BUILD_TYPE_DEBUG)
  target_compile_definitions(CodexEngine PUBLIC CX_BUILD_TYPE_DEBUG)
  target_compile_definitions(CodexEngine-static PUBLIC CX_BUILD_TYPE_DEBUG)
elseif(DEFINED CX_BUILD_TYPE_RELEASE)
  target_compile_definitions(CodexEngine PUBLIC CX_BUILD_TYPE_RELEASE)
  target_compile_definitions(CodexEngine-static PUBLIC CX_BUILD_TYPE_RELEASE)
elseif(DEFINED CX_BUILD_TYPE_SHIPPING)
  target_compile_definitions(CodexEngine PUBLIC CX_BUILD_TYPE_SHIPPING)
  target_compile_definitions(CodexEngine-static PUBLIC CX_BUILD_TYPE_SHIPPING)
endif()

# Print the platform information
message(":: Platform OS: ${CMAKE_SYSTEM_NAME}")
message(":: Platform Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(":: CPU word size: ${CMAKE_SIZEOF_VOID_P} bytes")
message("")

if(DEFINED CX_COMPILER_CLANG OR DEFINED CX_COMPILER_GCC)
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address
  # -fno-omit-frame-pointer") set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}
  # -fsanitize=address -fno-omit-frame-pointer")
endif()

# ============= # Libraries   # ============= #

# SDL
if(ANDROID)
  find_package(SDL2 REQUIRED)
  target_include_directories(CodexEngine PRIVATE ${SDL2_INCLUDE_DIRS})
  target_link_libraries(CodexEngine ${SDL2_LIBRARIES})

  target_include_directories(CodexEngine-static PRIVATE ${SDL2_INCLUDE_DIRS})
  target_link_libraries(CodexEngine-static ${SDL2_LIBRARIES})
else()
  add_subdirectory(vendor/SDL ${PROJECT_BINARY_DIR}/SDL EXCLUDE_FROM_ALL)
  set(SDL2_INCLUDE_DIRS vendor/SDL/include)
  set(SDL2_LIBRARIES SDL2 SDL2main)

  target_include_directories(CodexEngine PRIVATE ${SDL2_INCLUDE_DIRS})
  target_link_libraries(CodexEngine ${SDL2_LIBRARIES})
  add_dependencies(CodexEngine SDL2 SDL2main)

  target_include_directories(CodexEngine-static PRIVATE ${SDL2_INCLUDE_DIRS})
  target_link_libraries(CodexEngine-static ${SDL2_LIBRARIES})
endif()

# fmt
add_subdirectory(vendor/fmt ${PROJECT_BINARY_DIR}/fmt EXCLUDE_FROM_ALL)
set(FMT_INCLUDE_DIRS vendor/fmt/include)
set(FMT_LIBRARIES fmt)

target_include_directories(CodexEngine PRIVATE ${FMT_INCLUDE_DIRS})
target_link_libraries(CodexEngine ${FMT_LIBRARIES})

target_include_directories(CodexEngine-static PRIVATE ${FMT_INCLUDE_DIRS})
target_link_libraries(CodexEngine-static ${FMT_LIBRARIES})

# nlohmann json
add_subdirectory(vendor/json ${PROJECT_BINARY_DIR}/json EXCLUDE_FROM_ALL)
set(JSON_INCLUDE_DIRS vendor/json/include)
set(JSON_LIBRARIES nlohmann_json)

target_include_directories(CodexEngine PUBLIC ${JSON_INCLUDE_DIRS})
target_link_libraries(CodexEngine ${JSON_LIBRARIES})

target_include_directories(CodexEngine-static PUBLIC ${JSON_INCLUDE_DIRS})
target_link_libraries(CodexEngine-static ${JSON_LIBRARIES})

# glad
add_subdirectory(vendor/glad ${PROJECT_BINARY_DIR}/glad EXCLUDE_FROM_ALL)
set(GLAD_INCLUDE_DIRS vendor/glad)
set(GLAD_LIBRARIES glad)

target_include_directories(CodexEngine PUBLIC ${GLAD_INCLUDE_DIRS})
target_link_libraries(CodexEngine ${GLAD_LIBRARIES})

target_include_directories(CodexEngine-static PUBLIC ${GLAD_INCLUDE_DIRS})
target_link_libraries(CodexEngine-static ${GLAD_LIBRARIES})

# ImGui

# Define these so ImGui doesn't try and load OpenGL functions because we have
# GLAD for that.
target_compile_definitions(CodexEngine PRIVATE IMGUI_IMPL_OPENGL_LOADER_CUSTOM)
target_compile_definitions(CodexEngine-static
                           PRIVATE IMGUI_IMPL_OPENGL_LOADER_CUSTOM)

set(IMGUI_INCLUDE_DIRS vendor/imgui vendor/imgui/backends vendor/imgui/examples
                       vendor/imgui/misc)
file(
  GLOB_RECURSE
  IMGUI_HEADER_FILES
  "vendor/imgui/imconfig.h"
  "vendor/imgui/imgui.h"
  "vendor/imgui/imgui_internal.h"
  "vendor/imgui/imstb_rectpack.h"
  "vendor/imgui/imstb_textedit.h"
  "vendor/imgui/imstb_truetype.h")
file(
  GLOB_RECURSE
  IMGUI_SOURCE_FILES
  "vendor/imgui/imgui.cpp"
  "vendor/imgui/imgui_widgets.cpp"
  "vendor/imgui/imgui_draw.cpp"
  "vendor/imgui/imgui_demo.cpp"
  "vendor/imgui/imgui_tables.cpp"
  "vendor/imgui/backends/imgui_impl_opengl3.cpp"
  "vendor/imgui/backends/imgui_impl_sdl2.cpp")
target_sources(CodexEngine PUBLIC ${IMGUI_HEADER_FILES} ${IMGUI_SOURCE_FILES})
target_sources(CodexEngine-static PUBLIC ${IMGUI_HEADER_FILES}
                                         ${IMGUI_SOURCE_FILES})

target_include_directories(CodexEngine PUBLIC ${IMGUI_INCLUDE_DIRS})
target_include_directories(CodexEngine-static PUBLIC ${IMGUI_INCLUDE_DIRS})

# glm
add_subdirectory(vendor/glm ${PROJECT_BINARY_DIR}/glm EXCLUDE_FROM_ALL)
set(GLM_INCLUDE_DIRS vendor/glm/glm)
set(GLM_LIBRARIES glm)

target_include_directories(CodexEngine PUBLIC ${GLM_INCLUDE_DIRS})
target_link_libraries(CodexEngine ${GLM_LIBRARIES})

target_include_directories(CodexEngine-static PUBLIC ${GLM_INCLUDE_DIRS})
target_link_libraries(CodexEngine-static ${GLM_LIBRARIES})

# mgl
add_subdirectory(vendor/MGL/MGL ${PROJECT_BINARY_DIR}/MGL EXCLUDE_FROM_ALL)
set(MGL_INCLUDE_DIRS vendor/MGL/include)
set(MGL_LIBRARIES MGL)

target_include_directories(CodexEngine PUBLIC ${MGL_INCLUDE_DIRS})
target_link_libraries(CodexEngine ${MGL_LIBRARIES})

target_include_directories(CodexEngine-static PUBLIC ${MGL_INCLUDE_DIRS})
target_link_libraries(CodexEngine-static ${MGL_LIBRARIES})

# Net Native set(NETNT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/NetNT/src)
# target_include_directories(CodexEngine PUBLIC ${NETNT_INCLUDE_DIRS})
# target_link_libraries(CodexEngine NetNT)
# target_include_directories(CodexEngine-static PUBLIC ${NETNT_INCLUDE_DIRS})
# target_link_libraries(CodexEngine-static NetNT)

# Link OpenGL (IF UNIX LIKE)
find_package(OpenGL REQUIRED)
target_include_directories(CodexEngine PUBLIC ${OPENGL_INCLUDE_DIRS})
target_link_libraries(CodexEngine ${OPENGL_LIBRARIES})

target_include_directories(CodexEngine-static PUBLIC ${OPENGL_INCLUDE_DIRS})
target_link_libraries(CodexEngine-static ${OPENGL_LIBRARIES})

# entt
set(ENTT_INCLUDE_DIRS vendor/entt)
target_include_directories(CodexEngine PUBLIC ${ENTT_INCLUDE_DIRS})
target_include_directories(CodexEngine-static PUBLIC ${ENTT_INCLUDE_DIRS})

# ================================= # Configuration specific macros   #
# ================================= #
target_compile_definitions(
  CodexEngine
  PRIVATE $<$<CONFIG:Debug>:CX_CONFIG_DEBUG>
          $<$<CONFIG:Release>:CX_CONFIG_RELEASE>
          $<$<CONFIG:Shipping>:CX_CONFIG_SHIPPING>)
target_compile_definitions(
  CodexEngine-static
  PRIVATE $<$<CONFIG:Debug>:CX_CONFIG_DEBUG>
          $<$<CONFIG:Release>:CX_CONFIG_RELEASE>
          $<$<CONFIG:Shipping>:CX_CONFIG_SHIPPING>)

# ======================= # INSTALLATION # ======================= #
if(DEFINED CX_PLATFORM_WINDOWS)
  install(TARGETS SDL2 SDL2main glm glad nlohmann_json CodexEngine
          DESTINATION bin)
  install(TARGETS CodexEngine-static MGL DESTINATION "bin/Object Libraries")
  install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/assets/" DESTINATION bin)

  # Program Files (or same location as the binary) for unmodifiable file data.
  # App Data for modifidable file data.
elseif(DEFINED CX_PLATFORM_UNIX)
  install(
    TARGETS SDL2
            SDL2main
            glm
            glad
            nlohmann_json
            MGL
            CodexEngine
            CodexEngine-static
    DESTINATION lib)
  install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/assets/"
          DESTINATION share/${CMAKE_PROJECT_NAME})

  # /etc/appname for global configuration files. /usr/local/share/appname for
  # unmodifiable data. /usr/local/(lib, bin)/appname for unmodifiable and
  # machine dependent data. /var/local/(lib, bin)/appname for modifiable data.
endif()

# =============== # MISC      # =============== #

# Define standalone mode for the static library since the static version is
# going to be used for shipping target_compile_definitions(CodexEngine-static
# PRIVATE CX_MODE_STANDALONE 1)

# Make the base include file public
target_include_directories(CodexEngine PUBLIC include/)
target_include_directories(CodexEngine PUBLIC src/)

target_include_directories(CodexEngine-static PUBLIC include/)
target_include_directories(CodexEngine-static PUBLIC src/)

# Add the precompiled header
target_include_directories(CodexEngine PUBLIC src/PCH)
target_precompile_headers(CodexEngine PRIVATE src/PCH/sdafx.h)

target_include_directories(CodexEngine-static PUBLIC src/PCH)
target_precompile_headers(CodexEngine-static PRIVATE src/PCH/sdafx.h)

# Treat all warnings as errors for QUALITY
if(CX_COMPILER_MSVC)
  # target_compile_options(CodexEngine PRIVATE /W3 /WX)
else()
  # target_compile_options(CodexEngine PRIVATE -Wall -Wextra -Wno-error=extra
  # -Werror)
endif()
