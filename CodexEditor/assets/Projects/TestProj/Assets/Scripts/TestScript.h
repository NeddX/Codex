#pragma once

#include <Modex.h>

// To be added by Reflector
#include <Reflector.h>

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

    // To be added by Reflector
    void   Serialize() override;
    object GetField(const std::string_view name) override;

    // To be added by Reflector
    friend CODEX_EXPORT NativeBehaviour* Rf_CreateInstance(const char* className);
};
