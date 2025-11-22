#ifndef CODEX_CORE_GEOMETRY_DEFS_H
#define CODEX_CORE_GEOMETRY_DEFS_H

#include <sdafx.h>

#include <MGL.h>

namespace codex::math {
    using Matrix4f = glm::mat4;
    using Matrix2f = glm::mat2;
    using Vector2f = glm::vec2;
    using Vector3f = glm::vec3;
    using Vector4f = glm::vec4;
    using Vector2  = glm::ivec2;
    using Vector3  = glm::ivec3;
    using Vector4  = glm::ivec4;
    using Rect     = mgl::Rect;
    using Rectf    = mgl::Rectf;
} // namespace codex::math

using namespace codex::math;
#endif // CODEX_CORE_GEOMETRY_DEFS_H
