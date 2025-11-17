cmake_minimum_required (VERSION 3.8)

project("MGL")

add_subdirectory("MGL")
add_subdirectory("Sandbox")

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT Test)