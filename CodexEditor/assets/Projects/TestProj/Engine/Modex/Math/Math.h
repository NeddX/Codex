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

    template <typename T>
    constexpr T Clamp(const T& value, const T& min, const T& max)
    {
        return std::max(min, std::min(value, max));
    }

    constexpr f32 Clampf(const f32 value, const f32 min, const f32 max)
    {
        return std::max(min, std::min(value, max));
    }
} // namespace codex::math
