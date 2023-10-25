#ifndef CODEX_PCH_H
#define CODEX_PCH_H

// Turn this off.
// TODO: Disable compilers warnings in cmake.
#ifdef CX_COMPILER_MSVC
#pragma warning(disable: 4005)
#endif

// STL headers
#include <iostream>
#include <memory>
#include <chrono>
#include <thread>
#include <fstream>
#include <algorithm>
#include <utility>
#include <functional>
#include <type_traits>
#include <mutex>
#include <exception>
#include <regex>
#include <numeric>
#include <sstream>
#include <typeinfo>

// Legacy
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>

// STL Containers
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <bitset>
#include <array>
#include <initializer_list>

// GLAD
#include <glad/glad.h>

// Platform specific
#ifdef CX_PLATFORM_UNIX
#include <cxxabi.h>
#include <unistd.h>

#ifdef CX_PLATFORM_OSX
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

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

// Library specific
#include <fmt/core.h>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_syswm.h>
#include <SDL_opengl.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtx/rotate_vector.hpp> // glm::rotate(glm::vecX)
#include <MGL.h>
//#include <NetNative.h>
#include <entt.hpp>

// Project specific
#include "../Engine/Utils/include/Util.h"

#endif // CODEX_PCH_H
