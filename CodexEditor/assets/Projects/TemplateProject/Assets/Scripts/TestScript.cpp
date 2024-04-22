#include "TestScript.h"

void MienScripten::OnInit()
{
    std::cout << "Initialized baby!\n";
}

void MienScripten::OnUpdate(const f32 deltaTime)
{
    if (m_Move)
    {
        static float count     = 0;
        static auto& transform = GetComponent<TransformComponent>();
        Vector3f     temp      = (transform.position + std::sin(count)) * m_Axies * m_Multiplier * deltaTime;
        transform.position += temp;
        count += 0.1f;
    }
}