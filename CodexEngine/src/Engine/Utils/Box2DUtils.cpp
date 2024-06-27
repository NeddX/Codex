#include "Box2DUtils.h"

#include <sdafx.h>

namespace codex::utils {
    [[nodiscard]] b2BodyType ToB2Type(const RigidBody2DComponent::BodyType& type) noexcept
    {
        switch (type)
        {
            using enum RigidBody2DComponent::BodyType;

            case Static: return b2_staticBody;
            case Dynamic: return b2_dynamicBody;
            case Kinematic: return b2_kinematicBody;
        }

        CX_ASSERT(false, "Unknown body type.");
        return b2_staticBody;
    }

    [[nodiscard]] b2Vec3 ToB2Vec3(const Vector3f& vec) noexcept
    {
        return b2Vec3(vec.x, vec.y, vec.z);
    }

    [[nodiscard]] b2Vec2 ToB2Vec2(const Vector2f& vec) noexcept
    {
        return b2Vec2(vec.x, vec.y);
    }
} // namespace codex::utils
