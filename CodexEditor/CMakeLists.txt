project("CEditor")

# For running Python scripts from CMake.
find_package(
  Python3
  COMPONENTS Interpreter
  REQUIRED)

file(GLOB_RECURSE CEDITOR_SOURCE_FILES "src/*.cpp")
file(GLOB_RECURSE CEDITOR_HEADER_FILES "src/*.h")
file(GLOB_RECURSE CEDITOR_MODULE_FILES "src/*.ixx")

add_executable(CodexEditor ${CEDITOR_HEADER_FILES} ${CEDITOR_SOURCE_FILES}
                           ${CEDITOR_MODULE_FILES})

# Set the C++ Standard to 20 for this target
set_property(TARGET CodexEditor PROPERTY CXX_STANDARD 20)

target_link_libraries(CodexEditor CodexEngine)
add_dependencies(CodexEditor CodexEngine)

get_target_property(CODEX_BUILD_DIR CodexEngine BINARY_DIR)

get_target_property(CODEX_CMAKE_SOURCE_DIR CodexEngine SOURCE_DIR)

if (DEFINED CX_BUILD_TYPE_RELEASE OR DEFINED CX_BUILD_TYPE_SHIPPING)
  if (DEFINED CX_COMPILER_CLANG OR DEFINED CX_COMPILER_GNUC)
    target_compile_options(CodexEngine PRIVATE -Wall -Wextra -Werror -pedantic)
    target_compile_options(CodexEngine-static PRIVATE -Wall -Wextra -Werror -pedantic)
  elseif(DEFINED CX_COMPILER_MSVC)
  endif()
endif()

# tinyfiledialogs
set(TFDS_INCLUDE_DIRS vendor/tinyfiledialogs)
set(TFDS_SOURCE_FILES vendor/tinyfiledialogs/tinyfiledialogs.c)

target_include_directories(CodexEditor PUBLIC ${TFDS_INCLUDE_DIRS})
target_sources(CodexEditor PRIVATE ${TFDS_SOURCE_FILES})

# ImGuizmo
set(IMGUIZMO_INCLUDE_DIRS vendor/ImGuizmo)
set(IMGUIZMO_SOURCE_FILES vendor/ImGuizmo/ImGuizmo.cpp)

if(DEFINED CX_COMPILER_GCC OR DEFINED CX_COMPILER_CLANG)
  add_definitions(-DCE_INSTALL_DIR="${CMAKE_INSTALL_PREFIX}")
elseif(DEFINED CX_COMPILER_MSVC)
  add_definitions(/DCE_INSTALL_DIR="${CMAKE_INSTALL_PREFIX}")
endif()

# === Dependencies ===
if(DEFINED CX_PLATFORM_WINDOWS)
  install(TARGETS CodexEditor DESTINATION bin)
  install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/assets/" DESTINATION bin)
elseif(DEFINED CX_PLATFORM_UNIX)
  install(TARGETS CodexEditor DESTINATION bin)
  install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/assets/"
          DESTINATION share/${PROJECT_NAME})
endif()

target_include_directories(CodexEditor PUBLIC ${IMGUIZMO_INCLUDE_DIRS})
target_sources(CodexEditor PRIVATE ${IMGUIZMO_SOURCE_FILES})

# === Misc ===

# Run the python script resonsible for creating the template project.
add_custom_target(
  generate_template_project
  COMMAND
    ${Python3_EXECUTABLE} "${CX_ROOT_DIR}/Scripts/generate-template-project.py"
    "--src-dir=${CX_ENGINE_ROOT_DIR}"
    "--dest-dir=${CMAKE_CURRENT_SOURCE_DIR}/assets/Projects/TemplateProject"
    "--root-dir=${CX_ROOT_DIR}"
  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
  COMMENT "Generating Template Project...")

# If we're on Windows then we also need to copy the symbol libraries for linkage.
if(DEFINED CX_PLATFORM_WINDOWS)
  add_custom_command(
    TARGET generate_template_project
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/assets/Projects/TemplateProject/lib/"
    COMMAND
      ${CMAKE_COMMAND} -E copy "$<TARGET_FILE_DIR:CodexEngine>/CodexEngine.lib"
      "${CMAKE_CURRENT_SOURCE_DIR}/assets/Projects/TemplateProject/lib/CodexEngine.lib"
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:MGL>"
            "${CMAKE_CURRENT_SOURCE_DIR}/assets/Projects/TemplateProject/lib/"
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:fmt>"
            "${CMAKE_CURRENT_SOURCE_DIR}/assets/Projects/TemplateProject/lib/")
endif()

add_dependencies(CodexEditor generate_template_project)
target_include_directories(CodexEditor PRIVATE "src/")

# Multicore compilation for MSVC if CX_PARALLEL is not 0.
if(DEFINED CX_COMPILER_MSVC AND NOT CX_PARALLEL EQUAL 0)
  target_compile_options(CodexEditor PRIVATE "/MP")
endif()

# Set default startup project to be this one when generating a VS Solution.
set_target_properties(CodexEditor PROPERTIES VS_STARTUP_PROJECT CodexEditor)
