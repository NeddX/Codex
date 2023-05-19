add_executable (TestGame "src/main.cpp")

# Set the C++ Standard to 17 for this target
set_property(TARGET TestGame PROPERTY CXX_STANDARD 17)

target_link_libraries(TestGame CodexEngine-static)