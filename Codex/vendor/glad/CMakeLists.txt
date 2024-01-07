cmake_minimum_required(VERSION 3.20)
project("glad")

message(":: ------------------------ Hello, this is MGL's GLAD. --------------------------- ")

add_library(glad SHARED "glad.cpp" "glad.h" "khrplatform.h")
target_compile_definitions(glad PUBLIC GLAD_GLAPI_EXPORT)
target_compile_definitions(glad PRIVATE GLAD_GLAPI_EXPORT_BUILD)
