file(GLOB_RECURSE CODEXEDITOR_SOURCE_FILES "src/*.cpp")
file(GLOB_RECURSE CODEXEDITOR_HEADER_FILES "src/*.h")

add_executable (CodexEditor ${CODEXEDITOR_HEADER_FILES} ${CODEXEDITOR_SOURCE_FILES})

# Set the C++ Standard to 20 for this target
set_property(TARGET CodexEditor PROPERTY CXX_STANDARD 20)

target_link_libraries(CodexEditor CodexEngine)
add_dependencies(CodexEditor CodexEngine)

get_target_property(CODEX_BUILD_DIR CodexEngine BINARY_DIR)

# Function to copy the binaries of dependencies to the binary directory of a target
function(copy_dependencies_of TARGET_NAME)
    get_target_property(target_type ${TARGET_NAME} TYPE)
    if (target_type STREQUAL "SHARED_LIBRARY")
        get_target_property(target_dependencies ${TARGET_NAME} INTERFACE_LINK_LIBRARIES)
        foreach(dep ${target_dependencies})
            copy_dependencies_of(dep)
            add_custom_command(TARGET ${TARGET_NAME}
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${dep}> $<TARGET_FILE_DIR:${TARGET_NAME}>
                COMMENT ":: Copying ${dep} to ${TARGET_NAME} binary directory"
            )
        endforeach()
    endif()
endfunction()

get_target_property(CODEX_CMAKE_SOURCE_DIR CodexEngine SOURCE_DIR)
add_custom_command(
    TARGET CodexEditor
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SDL2> $<TARGET_FILE_DIR:CodexEditor>
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SDL2main> $<TARGET_FILE_DIR:CodexEditor>
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:CodexEngine> $<TARGET_FILE_DIR:CodexEditor>
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:CodexEngine-static> $<TARGET_FILE_DIR:CodexEditor>
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CODEX_CMAKE_SOURCE_DIR}/assets/" "${CMAKE_BINARY_DIR}/CodexEditor"
)
