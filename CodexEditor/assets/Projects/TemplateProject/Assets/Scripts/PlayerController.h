#pragma once

#include <Codex.h>

using namespace codex;

RF_CLASS()
class CODEX_EXPORT PlayerController : public NativeBehaviour
{
    RF_GENERATE_BODY()

private:
    RigidBody2DComponent* m_Rb2d   = nullptr;
    Entity                m_Camera = Entity::None();
    RF_SERIALIZABLE()
    Vector2f m_Velocity = { 20.0f, 20.0f };
    RF_SERIALIZABLE()
    Vector2f m_CurrentVelocity = { 0.0f, 0.0f };
    RF_SERIALIZABLE()
    Vector3f m_Sub = { 10.0f, 10.0f, 0.0f };
    RF_SERIALIZABLE()
    f32 m_Lerp = 0.5f;

public:
    void OnInit() override;
    void OnUpdate(const f32 deltaTime) override;
    void OnFixedUpdate(const f32 deltaTime) override;
};
