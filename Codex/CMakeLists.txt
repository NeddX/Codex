# Fetch all the source and header files and the then add them automatically
file(GLOB_RECURSE ENGINE_SOURCES "src/Engine/*.cpp")
file(GLOB_RECURSE ENGINE_HEADERS "src/Engine/*.h")

add_library(Codex-static STATIC ${ENGINE_HEADERS} ${ENGINE_SOURCES})
add_library(Codex SHARED ${ENGINE_HEADERS} ${ENGINE_SOURCES})


# Set the C++ Standard to 17 for this target
set_property(TARGET Codex PROPERTY CXX_STANDARD 17)
set_property(TARGET Codex-static PROPERTY CXX_STANDARD 17)

# ============= #
#   Libraries	#
# ============= #

# SDL
if (ANDROID)
	find_package(SDL2 REQUIRED)
	target_include_directories(Codex PRIVATE ${SDL2_INCLUDE_DIRS})
	target_link_libraries(Codex ${SDL2_LIBRARIES})
	target_link_libraries(Codex-static ${SDL2_LIBRARIES})
	target_include_directories(Codex-static PRIVATE ${SDL2_INCLUDE_DIRS})
	target_link_libraries(Codex-static ${SDL2_LIBRARIES})
else()
	add_subdirectory(vendor/SDL ${PROJECT_BINARY_DIR}/SDL)
	set(SDL2_INCLUDE_DIRS vendor/SDL/include)
	set(SDL2_LIBRARIES SDL2-static SDL2main)
	target_include_directories(Codex PRIVATE ${SDL2_INCLUDE_DIRS})
	target_include_directories(Codex-static PRIVATE ${SDL2_INCLUDE_DIRS})
	target_link_libraries(Codex ${SDL2_LIBRARIES})
	target_include_directories(Codex-static PRIVATE ${SDL2_INCLUDE_DIRS})
	target_link_libraries(Codex-static ${SDL2_LIBRARIES})
endif()

# fmt
add_subdirectory(vendor/fmt ${PROJECT_BINARY_DIR}/fmt)
set(FMT_INCLUDE_DIRS vendor/fmt/include)
set(FMT_LIBRARIES fmt)
target_include_directories(Codex PRIVATE ${FMT_INCLUDE_DIRS})
target_include_directories(Codex-static PRIVATE ${FMT_INCLUDE_DIRS})
target_link_libraries(Codex ${FMT_LIBRARIES})
target_include_directories(Codex-static PRIVATE ${FMT_INCLUDE_DIRS})
target_link_libraries(Codex-static ${FMT_LIBRARIES})

# yaml-cpp
add_subdirectory(vendor/yaml-cpp ${PROJECT_BINARY_DIR}/yaml-cpp)
set(YAMLCPP_INCLUDE_DIRS vendor/yaml-cpp/include)
set(YAMLCPP_LIBRARIES yaml-cpp)
target_include_directories(Codex PRIVATE ${YAMLCPP_INCLUDE_DIRS})
target_include_directories(Codex-static PRIVATE ${YAMLCPP_INCLUDE_DIRS})
target_link_libraries(Codex ${YAMLCPP_LIBRARIES})
target_include_directories(Codex-static PRIVATE ${YAMLCPP_INCLUDE_DIRS})
target_link_libraries(Codex-static ${YAMLCPP_LIBRARIES})

# glad
set(GLAD_INCLUDE_DIRS vendor/glad/include)
set(GLAD_SOURCE_FILE vendor/glad/src/glad.cpp)
target_include_directories(Codex PUBLIC ${GLAD_INCLUDE_DIRS})
target_include_directories(Codex-static PUBLIC ${GLAD_INCLUDE_DIRS})
target_sources(Codex PRIVATE ${GLAD_SOURCE_FILE})
target_include_directories(Codex-static PUBLIC ${GLAD_INCLUDE_DIRS})
target_sources(Codex-static PRIVATE ${GLAD_SOURCE_FILE})

# glm
add_subdirectory(vendor/glm ${PROJECT_BINARY_DIR}/glm)
set(GLM_INCLUDE_DIRS vendor/glm/glm)
set(GLM_LIBRARIES glm)
target_include_directories(Codex PRIVATE ${GLM_INCLUDE_DIRS})
target_include_directories(Codex-static PRIVATE ${GLM_INCLUDE_DIRS})
target_link_libraries(Codex ${GLM_LIBRARIES})
target_include_directories(Codex-static PRIVATE ${GLM_INCLUDE_DIRS})
target_link_libraries(Codex-static ${GLM_LIBRARIES})

# mgl
add_subdirectory(vendor/MGL/MGL ${PROJECT_BINARY_DIR}/MGL)
set(MGL_INCLUDE_DIRS vendor/MGL/include)
set(MGL_LIBRARIES MGL)
target_include_directories(Codex PUBLIC ${MGL_INCLUDE_DIRS})
target_include_directories(Codex-static PUBLIC ${MGL_INCLUDE_DIRS})
target_link_libraries(Codex ${MGL_LIBRARIES})
target_include_directories(Codex-static PUBLIC ${MGL_INCLUDE_DIRS})
target_link_libraries(Codex-static ${MGL_LIBRARIES})

# Net Native
set(NETNT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/NetNT/src)
target_include_directories(Codex PUBLIC ${NETNT_INCLUDE_DIRS})
target_include_directories(Codex-static PUBLIC ${NETNT_INCLUDE_DIRS})
target_link_libraries(Codex NetNT)
target_link_libraries(Codex-static NetNT)

# Link OpenGL and GLUT (IF UNIX LIKE)
find_package(OpenGL REQUIRED)
if (UNIX)
	find_package(GLUT REQUIRED)
endif()
target_include_directories(Codex PRIVATE ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
target_link_libraries(Codex ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
target_include_directories(Codex-static PRIVATE ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
target_link_libraries(Codex-static ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

# entt
set(ENTT_INCLUDE_DIRS vendor/entt)
target_include_directories(Codex PUBLIC ${ENTT_INCLUDE_DIRS})
target_include_directories(Codex-static PUBLIC ${ENTT_INCLUDE_DIRS})

# =============== #
#       MISC      #
# =============== #

# Define standalone mode for the static library since the static version is going to be used for shipping
target_compile_definitions(Codex-static PRIVATE CFX_MODE_STANDALONE 1)

# Make the base include file public
target_include_directories(Codex PUBLIC include/)
target_include_directories(Codex-static PUBLIC include/)

# Add the precompiled header
target_include_directories(Codex PUBLIC src/PCH)
target_precompile_headers(Codex PRIVATE src/PCH/sdafx.h)
target_include_directories(Codex-static PUBLIC src/PCH)
target_precompile_headers(Codex-static PRIVATE src/PCH/sdafx.h)

# BS
target_link_libraries(Codex ${CMAKE_DL_LIBS})
target_link_libraries(Codex-static ${CMAKE_DL_LIBS})

# Treat all warnings as errors for better code
if (MSVC)
	#target_compile_options(Codex PRIVATE /W3 /WX)
else()
	#target_compile_options(Codex PRIVATE -Wall -Wextra -Wno-error=extra -Werror)
endif()
