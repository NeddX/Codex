# Fetch all the source and header files and the then add them automatically
file(GLOB_RECURSE ENGINE_SOURCES "src/Engine/*.cpp")
file(GLOB_RECURSE ENGINE_HEADERS "src/Engine/*.h")

add_library(CodexEngine SHARED ${ENGINE_HEADERS} ${ENGINE_SOURCES})
add_library(CodexEngine-static STATIC ${ENGINE_HEADERS} ${ENGINE_SOURCES})

# Set the C++ Standard to 17 for this target.
set_property(TARGET CodexEngine PROPERTY CXX_STANDARD 20)
set_property(TARGET CodexEngine-static PROPERTY CXX_STANDARD 20)

# For exporting the symbols on windows.
if(DEFINED CX_PLATFORM_WINDOWS)
    set_target_properties(CodexEngine-static PROPERTIES OUTPUT_NAME "CodexEngine")
    set_target_properties(CodexEngine-static PROPERTIES COMPILE_PDB_NAME "CodexEngine-static")
endif()

# Position independent code for shared libraries.
if (DEFINED CX_PLATFORM_UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

target_compile_definitions(CodexEngine PRIVATE CX_BUILD_LIB)

# Define macros that describe the current platform.
if(DEFINED CX_PLATFORM_LINUX)
    target_compile_definitions(CodexEngine PRIVATE CX_PLATFORM_LINUX)
    target_compile_definitions(CodexEngine PRIVATE CX_PLATFORM_UNIX)
    target_compile_definitions(CodexEngine-static PRIVATE CX_PLATFORM_LINUX)
    target_compile_definitions(CodexEngine-static PRIVATE CX_PLATFORM_UNIX)
elseif(DEFINED CX_PLATFORM_OSX)
    target_compile_definitions(CodexEngine PRIVATE CX_PLATFORM_OSX)
    target_compile_definitions(CodexEngine PRIVATE CX_PLATFORM_UNIX)
    target_compile_definitions(CodexEngine-static PRIVATE CX_PLATFORM_OSX)
    target_compile_definitions(CodexEngine-static PRIVATE CX_PLATFORM_UNIX)
elseif(DEFINED CX_PLATFORM_WINDOWS)
    target_compile_definitions(CodexEngine PRIVATE CX_PLATFORM_WINDOWS)
    target_compile_definitions(CodexEngine-static PRIVATE CX_PLATFORM_WINDOWS)
else()
    target_compile_definitions(CodexEngine PRIVATE CX_PLATFORM_UNKNOWN)
    target_compile_definitions(CodexEngine-static PRIVATE CX_PLATFORM_UNKNOWN)
endif()

if(DEFINED CX_COMPILER_GNUC)
    target_compile_definitions(CodexEngine PRIVATE CX_COMPILER_GNUC)
    target_compile_definitions(CodexEngine-static PRIVATE CX_COMPILER_GNUC)
elseif(DEFINED CX_COMPILER_CLANG)
    target_compile_definitions(CodexEngine PRIVATE CX_COMPILER_CLANG)
    target_compile_definitions(CodexEngine-static PRIVATE CX_COMPILER_CLANG)
elseif(DEFINED CX_COMPILER_MSVC)
    target_compile_definitions(CodexEngine PRIVATE CX_COMPILER_MSVC)
    target_compile_definitions(CodexEngine-static PRIVATE CX_COMPILER_MSVC)
else()
    target_compile_definitions(CodexEngine PRIVATE CX_COMPILER_UNKNOWN)
    target_compile_definitions(CodexEngine-static PRIVATE CX_COMPILER_UNKNOWN)
endif()

# Determine the processor architecture
if(DEFINED CX_WORD_SIZE_64)
    target_compile_definitions(CodexEngine PRIVATE CX_WORD_SIZE_64)
    target_compile_definitions(CodexEngine-static PRIVATE CX_WORD_SIZE_64)
else()
    target_compile_definitions(CodexEngine PRIVATE CX_WORD_SIZE_32)
    target_compile_definitions(CodexEngine-static PRIVATE CX_WORD_SIZE_32)
endif()

# Print the platform information
message(":: Platform OS: ${CMAKE_SYSTEM_NAME}")
message(":: Platform Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(":: Architecture word size: ${CMAKE_SIZEOF_VOID_P} bytes")

if(DEFINED CX_COMPILER_CLANG OR DEFINED CX_COMPILER_GCC)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
endif()

# ============= #
#   Libraries   #
# ============= #

# SDL
if (ANDROID)
    find_package(SDL2 REQUIRED)
    target_include_directories(CodexEngine PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(CodexEngine ${SDL2_LIBRARIES})

    target_include_directories(CodexEngine-static PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(CodexEngine-static ${SDL2_LIBRARIES})
else()
    add_subdirectory(vendor/SDL ${PROJECT_BINARY_DIR}/SDL)
    set(SDL2_INCLUDE_DIRS vendor/SDL/include)
    set(SDL2_LIBRARIES SDL2 SDL2main)

    target_include_directories(CodexEngine PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(CodexEngine ${SDL2_LIBRARIES})
    add_dependencies(CodexEngine SDL2 SDL2main)

    target_include_directories(CodexEngine-static PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(CodexEngine-static ${SDL2_LIBRARIES})

    add_custom_command(
        TARGET CodexEngine
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:SDL2>
            $<TARGET_FILE_DIR:CodexEngine>/$<TARGET_FILE_NAME:SDL2>
        COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:SDL2main>
            $<TARGET_FILE_DIR:CodexEngine>/$<TARGET_FILE_NAME:SDL2main>
    )

    add_custom_command(
        TARGET CodexEngine-static
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:SDL2>
            $<TARGET_FILE_DIR:CodexEngine-static>/$<TARGET_FILE_NAME:SDL2>
        COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:SDL2main>
            $<TARGET_FILE_DIR:CodexEngine-static>/$<TARGET_FILE_NAME:SDL2main>
    )

endif()

# fmt
add_subdirectory(vendor/fmt ${PROJECT_BINARY_DIR}/fmt)
set(FMT_INCLUDE_DIRS vendor/fmt/include)
set(FMT_LIBRARIES fmt)

target_include_directories(CodexEngine PRIVATE ${FMT_INCLUDE_DIRS})
target_link_libraries(CodexEngine ${FMT_LIBRARIES})

target_include_directories(CodexEngine-static PRIVATE ${FMT_INCLUDE_DIRS})
target_link_libraries(CodexEngine-static ${FMT_LIBRARIES})

# glad
set(GLAD_INCLUDE_DIRS vendor/glad/include)
set(GLAD_SOURCE_FILES vendor/glad/src/glad.cpp)

target_include_directories(CodexEngine PUBLIC ${GLAD_INCLUDE_DIRS})
target_sources(CodexEngine PRIVATE ${GLAD_SOURCE_FILES})

target_include_directories(CodexEngine-static PUBLIC ${GLAD_INCLUDE_DIRS})
target_sources(CodexEngine-static PRIVATE ${GLAD_SOURCE_FILES})

# ImGui
set(IMGUI_INCLUDE_DIRS vendor/imgui vendor/imgui/backends vendor/imgui/examples vendor/imgui/misc)
file(GLOB_RECURSE IMGUI_HEADER_FILES
    "vendor/imgui/imconfig.h"
    "vendor/imgui/imgui.h"
    "vendor/imgui/imgui_internal.h"
    "vendor/imgui/imstb_rectpack.h"
    "vendor/imgui/imstb_textedit.h"
    "vendor/imgui/imstb_truetype.h")
file(GLOB_RECURSE IMGUI_SOURCE_FILES
    "vendor/imgui/imgui.cpp"
    "vendor/imgui/imgui_widgets.cpp"
    "vendor/imgui/imgui_draw.cpp"
    "vendor/imgui/imgui_demo.cpp"
    "vendor/imgui/imgui_tables.cpp"
    "vendor/imgui/backends/imgui_impl_opengl3.cpp"
    "vendor/imgui/backends/imgui_impl_sdl2.cpp")
target_sources(CodexEngine PRIVATE ${IMGUI_HEADER_FILES} ${IMGUI_SOURCE_FILES})
target_sources(CodexEngine-static PRIVATE ${IMGUI_HEADER_FILES} ${IMGUI_SOURCE_FILES})

target_include_directories(CodexEngine PUBLIC ${IMGUI_INCLUDE_DIRS})
target_include_directories(CodexEngine-static PRIVATE ${IMGUI_INCLUDE_DIRS})

# glm
add_subdirectory(vendor/glm ${PROJECT_BINARY_DIR}/glm)
set(GLM_INCLUDE_DIRS vendor/glm/glm)
set(GLM_LIBRARIES glm)

target_include_directories(CodexEngine PRIVATE ${GLM_INCLUDE_DIRS})
target_link_libraries(CodexEngine ${GLM_LIBRARIES})

target_include_directories(CodexEngine-static PRIVATE ${GLM_INCLUDE_DIRS})
target_link_libraries(CodexEngine-static ${GLM_LIBRARIES})

# mgl
add_subdirectory(vendor/MGL/MGL ${PROJECT_BINARY_DIR}/MGL)
set(MGL_INCLUDE_DIRS vendor/MGL/include)
set(MGL_LIBRARIES MGL)

target_include_directories(CodexEngine PUBLIC ${MGL_INCLUDE_DIRS})
target_link_libraries(CodexEngine ${MGL_LIBRARIES})

target_include_directories(CodexEngine-static PUBLIC ${MGL_INCLUDE_DIRS})
target_link_libraries(CodexEngine-static ${MGL_LIBRARIES})

# Net Native
#set(NETNT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/NetNT/src)
#target_include_directories(CodexEngine PUBLIC ${NETNT_INCLUDE_DIRS})
#target_link_libraries(CodexEngine NetNT)
#target_include_directories(CodexEngine-static PUBLIC ${NETNT_INCLUDE_DIRS})
#target_link_libraries(CodexEngine-static NetNT)

# Link OpenGL and GLUT (IF UNIX LIKE)
find_package(OpenGL REQUIRED)
if (UNIX)
    find_package(GLUT REQUIRED)
endif()
target_include_directories(CodexEngine PRIVATE ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
target_link_libraries(CodexEngine ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

target_include_directories(CodexEngine-static PRIVATE ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
target_link_libraries(CodexEngine-static ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

# entt
set(ENTT_INCLUDE_DIRS vendor/entt)
target_include_directories(CodexEngine PUBLIC ${ENTT_INCLUDE_DIRS})
target_include_directories(CodexEngine-static PUBLIC ${ENTT_INCLUDE_DIRS})

# ================================= #
#   Configuration specific macros   #
# ================================= #
target_compile_definitions(CodexEngine PRIVATE
    $<$<CONFIG:Debug>:CX_CONFIG_DEBUG>
    $<$<CONFIG:Release>:CX_CONFIG_RELEASE>
    $<$<CONFIG:Shipping>:CX_CONFIG_SHIPPING>
)
target_compile_definitions(CodexEngine-static PRIVATE
    $<$<CONFIG:Debug>:CX_CONFIG_DEBUG>
    $<$<CONFIG:Release>:CX_CONFIG_RELEASE>
    $<$<CONFIG:Shipping>:CX_CONFIG_SHIPPING>
)

# =============== #
#       MISC      #
# =============== #

# Define standalone mode for the static library since the static version is going to be used for shipping
#target_compile_definitions(CodexEngine-static PRIVATE CX_MODE_STANDALONE 1)

# Make the base include file public
target_include_directories(CodexEngine PUBLIC include/)
target_include_directories(CodexEngine PUBLIC src/)

target_include_directories(CodexEngine-static PUBLIC include/)
target_include_directories(CodexEngine-static PUBLIC src/Engine)

# Add the precompiled header
target_include_directories(CodexEngine PUBLIC src/PCH)
#target_precompile_headers(CodexEngine PRIVATE src/PCH/sdafx.h)

target_include_directories(CodexEngine-static PUBLIC src/PCH)
#target_precompile_headers(CodexEngine-static PRIVATE src/PCH/sdafx.h)

# No idea what this is.
#target_link_libraries(CodexEngine-static ${CMAKE_DL_LIBS})
#target_link_libraries(CodexEngine ${CMAKE_DL_LIBS})

# Treat all warnings as errors for QUALITY
if (CX_COMPILER_MSVC)
    #target_compile_options(CodexEngine PRIVATE /W3 /WX)
else()
    #target_compile_options(CodexEngine PRIVATE -Wall -Wextra -Wno-error=extra -Werror)
endif()

