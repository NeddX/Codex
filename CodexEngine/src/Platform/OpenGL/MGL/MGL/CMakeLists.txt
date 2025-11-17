# Fetch all the source and header files and the then add them automatically
file(GLOB_RECURSE CORE_SOURCES "src/*.cpp")
file(GLOB_RECURSE CORE_HEADERS "src/*.h")

# Add the executable
add_library(MGL STATIC ${CORE_HEADERS} ${CORE_SOURCES})

# Set the C++ Standard to 17 for this target
set_property(TARGET MGL PROPERTY CXX_STANDARD 17)

# ============= #
#   Libraries	#
# ============= #

# glad
set(GLAD_INCLUDE_DIR vendor/glad)
target_include_directories(MGL PUBLIC ${GLAD_INCLUDE_DIR})
target_sources(MGL PUBLIC ${GLAD_INCLUDE_DIR}/glad.cpp)

# stb
target_include_directories(MGL PUBLIC vendor/stb)

# glm
add_subdirectory(vendor/glm ${PROJECT_BINARY_DIR}/glm)
set(GLM_INCLUDE_DIR vendor/glm/glm)
set(GLM_LIBRARIES glm)
target_include_directories(MGL PUBLIC ${GLM_INCLUDE_DIR})
target_link_libraries(MGL ${GLM_LIBRARIES})

# ============= #
#     MISC  	#
# ============= #
target_include_directories(MGL PUBLIC ${PROJECT_SOURCE_DIR}/include/)
