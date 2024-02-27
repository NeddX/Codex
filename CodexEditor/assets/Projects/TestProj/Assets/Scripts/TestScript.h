#pragma once

#include <Codex.h>

using namespace codex;

// This should be auto-generated.
#define MYPROJECT_API __declspec(dllexport)

RF_CLASS()
class MYPROJECT_API MienScripten : public NativeBehaviour
{
    RF_GENERATE_BODY()

private:
    RF_SERIALIZABLE()
    bool m_Move = false;
    RF_SERIALIZABLE()
    f32 m_Multiplier = 1.0f;
    RF_SERIALIZABLE()
    Vector3f m_Axies = { 0.0f, 1.0f, 0.0f };

public:
    void Init() override;
    void Update(const f32 deltaTime) override;
};
