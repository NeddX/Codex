#pragma once

#include <Modex.h>

#define RF_CLASS(...)
#define RF_SERIALIZE(...)

using namespace codex;

RF_CLASS()
class CODEX_EXPORT MienScripten : public NativeBehaviour
{
private:
    RF_SERIALIZE()
    bool m_Move = false;
    RF_SERIALIZE()
    f32 m_Multiplier = 1.0f;
    RF_SERIALIZE()
    Vector3f m_Axies = { 0.0f, 1.0f, 0.0f };

public:
    void   Init() override;
    void   Update(const f32 deltaTime) override;
    void   Serialize() override;
    object GetField(const std::string_view name) override;

    friend CODEX_EXPORT NativeBehaviour* Reflect_CreateBehaviour(const char* className, const Entity owner);
};
