# Fetch all the source and header files and the then add them automatically
file(GLOB_RECURSE ENGINE_SOURCES "src/Engine/*.cpp")
file(GLOB_RECURSE ENGINE_HEADERS "src/Engine/*.h")

add_library(CodexEngine ${ENGINE_HEADERS} ${ENGINE_SOURCES})
#add_library(CodexEngine-static STATIC ${ENGINE_HEADERS} ${ENGINE_SOURCES})

# Set the C++ Standard to 17 for this target
set_property(TARGET CodexEngine PROPERTY CXX_STANDARD 17)
#set_property(TARGET CodexEngine-static PROPERTY CXX_STANDARD 17)

# ============= #
#   Libraries   #
# ============= #

# SDL
if (ANDROID)
    find_package(SDL2 REQUIRED)
    target_include_directories(CodexEngine PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(CodexEngine ${SDL2_LIBRARIES})

    #target_link_libraries(CodexEngine-static ${SDL2_LIBRARIES})
    #target_include_directories(CodexEngine-static PRIVATE ${SDL2_INCLUDE_DIRS})
    #target_link_libraries(CodexEngine-static ${SDL2_LIBRARIES})
else()
    add_subdirectory(vendor/SDL ${PROJECT_BINARY_DIR}/SDL)
    set(SDL2_INCLUDE_DIRS vendor/SDL/include)
    set(SDL2_LIBRARIES SDL2-static SDL2main)
    target_include_directories(CodexEngine PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(CodexEngine ${SDL2_LIBRARIES})

    #target_include_directories(CodexEngine-static PRIVATE ${SDL2_INCLUDE_DIRS})
    #target_link_libraries(CodexEngine-static ${SDL2_LIBRARIES})
endif()

# fmt
add_subdirectory(vendor/fmt ${PROJECT_BINARY_DIR}/fmt)
set(FMT_INCLUDE_DIRS vendor/fmt/include)
set(FMT_LIBRARIES fmt)
target_include_directories(CodexEngine PRIVATE ${FMT_INCLUDE_DIRS})
target_link_libraries(CodexEngine ${FMT_LIBRARIES})

#target_include_directories(CodexEngine-static PRIVATE ${FMT_INCLUDE_DIRS})
#target_link_libraries(CodexEngine-static ${FMT_LIBRARIES})

# glad
set(GLAD_INCLUDE_DIRS vendor/glad/include)
set(GLAD_SOURCE_FILE vendor/glad/src/glad.cpp)
target_include_directories(CodexEngine PUBLIC ${GLAD_INCLUDE_DIRS})
target_sources(CodexEngine PRIVATE ${GLAD_SOURCE_FILE})

#target_include_directories(CodexEngine-static PUBLIC ${GLAD_INCLUDE_DIRS})
#target_sources(CodexEngine-static PRIVATE ${GLAD_SOURCE_FILE})

# glm
add_subdirectory(vendor/glm ${PROJECT_BINARY_DIR}/glm)
set(GLM_INCLUDE_DIRS vendor/glm/glm)
set(GLM_LIBRARIES glm)
target_include_directories(CodexEngine PRIVATE ${GLM_INCLUDE_DIRS})
target_link_libraries(CodexEngine ${GLM_LIBRARIES})

#target_include_directories(CodexEngine-static PRIVATE ${GLM_INCLUDE_DIRS})
#target_link_libraries(CodexEngine-static ${GLM_LIBRARIES})

# mgl
add_subdirectory(vendor/MGL/MGL ${PROJECT_BINARY_DIR}/MGL)
set(MGL_INCLUDE_DIRS vendor/MGL/include)
set(MGL_LIBRARIES MGL)
target_include_directories(CodexEngine PUBLIC ${MGL_INCLUDE_DIRS})
target_link_libraries(CodexEngine ${MGL_LIBRARIES})

#target_include_directories(CodexEngine-static PUBLIC ${MGL_INCLUDE_DIRS})
#target_link_libraries(CodexEngine-static ${MGL_LIBRARIES})

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

#target_include_directories(CodexEngine-static PRIVATE ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
#target_link_libraries(CodexEngine-static ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

# entt
set(ENTT_INCLUDE_DIRS vendor/entt)
target_include_directories(CodexEngine PUBLIC ${ENTT_INCLUDE_DIRS})
#target_include_directories(CodexEngine-static PUBLIC ${ENTT_INCLUDE_DIRS})

# ================================= #
#   Configuration specific macros   #
# ================================= #
target_compile_definitions(CodexEngine PRIVATE
    $<$<CONFIG:Debug>:CODEX_CONF_DEBUG>
    $<$<CONFIG:Release>:CODEX_CONF_RELEASE>
    $<$<CONFIG:Shipping>:CODEX_CONF_SHIPPING>
)
#target_compile_definitions(CodexEngine-static PRIVATE
#    $<$<CONFIG:Debug>:CODEX_CONF_DEBUG>
#    $<$<CONFIG:Release>:CODEX_CONF_RELEASE>
#    $<$<CONFIG:Shipping>:CODEX_CONF_SHIPPING>
#)

# =============== #
#       MISC      #
# =============== #

# Define standalone mode for the static library since the static version is going to be used for shipping
#target_compile_definitions(CodexEngine-static PRIVATE CX_MODE_STANDALONE 1)

# Make the base include file public
target_include_directories(CodexEngine PUBLIC include/)
#target_include_directories(CodexEngine-static PUBLIC include/)

# Add the precompiled header
target_include_directories(CodexEngine PUBLIC src/PCH)
#target_precompile_headers(CodexEngine PRIVATE src/PCH/sdafx.h)
#target_include_directories(CodexEngine-static PUBLIC src/PCH)
#target_precompile_headers(CodexEngine-static PRIVATE src/PCH/sdafx.h)

# No idea what this is.
#target_link_libraries(CodexEngine-static ${CMAKE_DL_LIBS})
#target_link_libraries(CodexEngine ${CMAKE_DL_LIBS})

# Treat all warnings as errors for QUALITY
if (MSVC)
    target_compile_options(CodexEngine PRIVATE /W3 /WX)
else()
    #target_compile_options(CodexEngine PRIVATE -Wall -Wextra -Wno-error=extra -Werror)
endif()

