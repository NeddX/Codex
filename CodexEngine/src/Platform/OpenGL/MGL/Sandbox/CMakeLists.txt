# Link the executable
add_executable (Sandbox "main.cpp")

# Copy neccessary files to the output binary folder
configure_file(${CMAKE_SOURCE_DIR}/Sandbox/assets/images/box.png ${CMAKE_BINARY_DIR}/Sandbox COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/Sandbox/assets/shaders/default.glsl ${CMAKE_BINARY_DIR}/Sandbox COPYONLY)

# Set the C++ Standard to 17 for this target
set_property(TARGET Sandbox PROPERTY CXX_STANDARD 17)

# Link MGL to this guy
target_link_libraries(Sandbox MGL)
