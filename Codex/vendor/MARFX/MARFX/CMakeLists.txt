# Fetch all the source and header files and the then add them automatically
file(GLOB_RECURSE CORE_SOURCES "src/*.cpp")
file(GLOB_RECURSE CORE_HEADERS "src/*.h")

# Add the executable
add_library(MARFX ${CORE_HEADERS} ${CORE_SOURCES})

# Set the C++ Standard to 17 for this target
set_property(TARGET MARFX PROPERTY CXX_STANDARD 17)

# ============= #
#   Libraries	#
# ============= #

# SDL
if (ANDROID)
	find_package(SDL2 REQUIRED)
	target_include_directories(MARFX PRIVATE ${SDL2_INCLUDE_DIRS})
	target_link_libraries(MARFX ${SDL2_LIBRARIES})
else()
	if (NOT SDL2_LIBRARIES)
		add_subdirectory(vendor/SDL ${PROJECT_BINARY_DIR}/SDL)
		set(SDL2_INCLUDE_DIRS vendor/SDL/include)
		set(SDL2_LIBRARIES SDL2-static SDL2main)
		target_include_directories(MARFX PRIVATE ${SDL2_INCLUDE_DIRS})
		target_link_libraries(MARFX ${SDL2_LIBRARIES})
	else()
		target_include_directories(MARFX PRIVATE ${SDL2_INCLUDE_DIRS})
		target_link_libraries(MARFX ${SDL2_LIBRARIES})
	endif()
endif()

# glad
if (NOT GLAD_SOURCE_FILE)
	target_include_directories(MARFX PUBLIC vendor/glad/include)
	target_sources(MARFX PRIVATE vendor/glad/src/glad.cpp)
else()
	target_include_directories(MARFX PRIVATE ${GLAD_INCLUDE_DIRS})
	target_sources(MARFX PRIVATE ${GLAD_SOURCE_FILE})
endif()

# stb
if (NOT STB_INCLUDE_DIRS)
	target_include_directories(MARFX PRIVATE vendor/stb)
else()
	target_include_directories(MARFX PRIVATE ${STB_INCLUDE_DIRS})
endif()

# glm
if (NOT GLM_LIBRARIES)
	add_subdirectory(vendor/glm ${PROJECT_BINARY_DIR}/glm)
	set(GLM_INCLUDE_DIRS vendor/glm/glm)
	set(GLM_LIBRARIES glm)
	target_include_directories(MARFX PRIVATE ${GLM_INCLUDE_DIRS})
	target_link_libraries(MARFX ${GLM_LIBRARIES})
else()
	target_include_directories(MARFX PRIVATE ${GLM_INCLUDE_DIRS})
	target_link_libraries(MARFX ${GLM_LIBRARIES})
endif()

# ============= #
#     MISC  	#
# ============= #
target_include_directories(MARFX PUBLIC ${PROJECT_SOURCE_DIR}/include/)