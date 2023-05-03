# Fetch all the source and header files and the then add them automatically
file(GLOB_RECURSE CORE_SOURCES "src/*.cpp")
file(GLOB_RECURSE CORE_HEADERS "src/*.h")

# Add the executable
add_executable (Sandbox "main.cpp" ${CORE_HEADERS} ${CORE_SOURCES})

# Copy the neccessary files to the binary folder
configure_file(${CMAKE_SOURCE_DIR}/Sandbox/assets/images/aws.png ${CMAKE_BINARY_DIR}/Sandbox COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/Sandbox/assets/images/box.png ${CMAKE_BINARY_DIR}/Sandbox COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/Sandbox/assets/shaders/default.glsl ${CMAKE_BINARY_DIR}/Sandbox COPYONLY)

# Set the C++ Standard to 17 for this target
set_property(TARGET Sandbox PROPERTY CXX_STANDARD 17)

# Link MARFX to this guy
target_link_libraries(Sandbox MARFX)
