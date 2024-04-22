#pragma once

#include <Codex.h>

using namespace codex;

RF_CLASS()
class CODEX_EXPORT MienScripten : public NativeBehaviour
{
    RF_GENERATE_BODY()

private:
    RF_SERIALIZABLE()
    bool m_Move = true;
    RF_SERIALIZABLE()
    f32 m_Multiplier = 1.0f;
    RF_SERIALIZABLE()
    Vector3f m_Axies = { 0.0f, 1.0f, 0.0f };

public:
    void OnInit() override;
    void OnUpdate(const f32 deltaTime) override;
};
