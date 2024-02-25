project("CEditor")

file(GLOB_RECURSE CODEXEDITOR_SOURCE_FILES "src/*.cpp")
file(GLOB_RECURSE CODEXEDITOR_HEADER_FILES "src/*.h")
file(GLOB_RECURSE CODEXEDITOR_MODULE_FILES "src/*.ixx")

add_executable(CodexEditor ${CODEXEDITOR_HEADER_FILES}
                           ${CODEXEDITOR_SOURCE_FILES}
                           ${CODEXEDITOR_MODULE_FILES})

# Set the C++ Standard to 20 for this target
set_property(TARGET CodexEditor PROPERTY CXX_STANDARD 20)

target_link_libraries(CodexEditor CodexEngine)
add_dependencies(CodexEditor CodexEngine)

get_target_property(CODEX_BUILD_DIR CodexEngine BINARY_DIR)

get_target_property(CODEX_CMAKE_SOURCE_DIR CodexEngine SOURCE_DIR)

# tinyfiledialogs
set(TFDS_INCLUDE_DIRS vendor/tinyfiledialogs)
set(TFDS_SOURCE_FILES vendor/tinyfiledialogs/tinyfiledialogs.c)

target_include_directories(CodexEditor PUBLIC ${TFDS_INCLUDE_DIRS})
target_sources(CodexEditor PRIVATE ${TFDS_SOURCE_FILES})

# ImGuizmo
set(IMGUIZMO_INCLUDE_DIRS vendor/ImGuizmo)
set(IMGUIZMO_SOURCE_FILES vendor/ImGuizmo/ImGuizmo.cpp)

add_definitions(-DCE_INSTALL_DIR="${CMAKE_INSTALL_PREFIX}")

# Dependencies
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

# Misc
target_include_directories(CodexEditor PRIVATE "src/")

# Multicore compilation for MSVC if CX_PARALLEL is not 0.
if(DEFINED CX_COMPILER_MSVC AND NOT CX_PARALLEL EQUAL 0)
    target_compile_options(CodexEditor PRIVATE "/MP")
endif()

# Set default startup project to be this one when generating a VS Solution.
set_target_properties(CodexEditor PROPERTIES VS_STARTUP_PROJECT CodexEditor)
