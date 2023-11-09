#pragma once

#include "../Core/CommonDef.h"

#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/vec2.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4

namespace codex::math {
    constexpr f32 PI = 3.14159265358979323f;

    constexpr f32 ToRadf(const f32 degree)
    {
        return degree * (PI / 180.0f);
    }

    bool TransformDecompose(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);
} // namespace codex::math
