#include "PlayerController.h"

void PlayerController::OnInit()
{
    m_Rb2d   = &GetComponent<RigidBody2DComponent>();
    m_Camera = GetPrimaryCameraEntity();
}

void PlayerController::OnUpdate(const f32 deltaTime)
{
}

void PlayerController::OnFixedUpdate(const f32 deltaTime)
{
    auto& camera    = m_Camera.GetComponent<CameraComponent>();
    auto& cam_tc    = m_Camera.GetComponent<TransformComponent>();
    auto& tc        = GetTransform();
    auto  lerp_pos  = glm::mix(tc.position, cam_tc.position, m_Lerp);
    cam_tc.position = lerp_pos - m_Sub;
     cam_tc.position =
         Vector3f{ lerp_pos.x - camera.camera.GetWidth() / 2, lerp_pos.y - camera.camera.GetHeight() / 2, 0.0f };

    m_CurrentVelocity = { 0.0f, 0.0f };
    if (Input::IsKeyDown(Key::W))
        m_CurrentVelocity.y = m_Velocity.y;
    if (Input::IsKeyDown(Key::A))
        m_CurrentVelocity.x = m_Velocity.x * -1;
    if (Input::IsKeyDown(Key::S))
        m_CurrentVelocity.y = m_Velocity.y * -1;
    if (Input::IsKeyDown(Key::D))
        m_CurrentVelocity.x = m_Velocity.x;
    if (Input::IsKeyDown(Key::Left))
        m_Rb2d->ApplyTorque(1.0f);
    if (Input::IsKeyDown(Key::Right))
        m_Rb2d->ApplyTorque(-1.0f);

    m_Rb2d->ApplyForce(m_CurrentVelocity);
}
