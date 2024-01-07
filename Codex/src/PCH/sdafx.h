#ifndef CODEX_PCH_H
#define CODEX_PCH_H

// Turn this off.
// TODO: Disable compilers warnings in cmake.
#ifdef CX_COMPILER_MSVC
#pragma warning(disable : 4005)
#endif

// STL headers
#include <algorithm>
#include <chrono>
#include <exception>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <numeric>
#include <regex>
#include <sstream>
#include <type_traits>
#include <typeinfo>
#include <utility>
#include <variant>

// Concurrency
#include <atomic>
#include <mutex>
#include <thread>

// Legacy
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

// STL Containers
#include <array>
#include <bitset>
#include <initializer_list>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

// GLAD
#include <glad.h>

// Platform specific
#ifdef CX_PLATFORM_UNIX
#include <cxxabi.h>
#include <dlfcn.h>
#include <unistd.h>

#ifdef CX_PLATFORM_OSX
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#else
#include <GL/gl.h>
#include <GL/glu.h>

#endif

#elif defined(CX_PLATFORM_WINDOWS)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#ifdef CreateWindow
#undef CreateWindow
#endif

#endif

// Codex specific
#include "../Engine/Core/CommonDef.h"
#include "../Engine/Math/Math.h"

// Library specific
#include <fmt/core.h>
#define SDL_MAIN_HANDLED
#include <MGL.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_syswm.h>
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/matrix_transform.hpp>  // glm::translate, glm::rotate, glm::scale
#include <glm/ext/scalar_constants.hpp>  // glm::pi
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/rotate_vector.hpp> // glm::rotate(glm::vecX)
#include <glm/mat4x4.hpp>            // glm::mat4
#include <glm/vec2.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
// #include <NetNative.h>
#include <entt.hpp>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <nlohmann/ordered_map.hpp>

// Project specific
#include "../Engine/Utils/include/Util.h"

// Stupid define by X11.
#ifdef None
#undef None
#endif

#endif // CODEX_PCH_H
