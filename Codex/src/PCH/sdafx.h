#ifndef CODEX_PCH_H
#define CODEX_PCH_H

// Turn this off.
#ifdef _MSC_VER
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
#ifdef __APPLE__
#include <unistd.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef CreateWindow
//#include <corecrt_io.h>
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include <unistd.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#endif

// Codex specific
#include "../Engine/Core/CommonDef.h"

// Library specific
#include <fmt/core.h>
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
