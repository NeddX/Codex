#ifndef CODEX_UTILS_BOX2D_UTILITIES_H
#define CODEX_UTILS_BOX2D_UTILITIES_H

#include <Engine/Core/CommonDef.h>
#include <Engine/Scene/Components.h>

namespace codex::utils {
    [[nodiscard]] CODEX_API b2BodyType ToB2Type(const RigidBody2DComponent::BodyType& type) noexcept;

    [[nodiscard]] CODEX_API b2Vec3 ToB2Vec3(const Vector3f& vec) noexcept;

    [[nodiscard]] CODEX_API b2Vec2 ToB2Vec2(const Vector2f& vec) noexcept;
} // namespace codex::utils

#endif // CODEX_UTILS_BOX2D_UTILITIES_H
