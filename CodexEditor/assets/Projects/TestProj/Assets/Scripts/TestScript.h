#pragma once

#include <Modex.h>

#define CX_SERIALIZE() ;
#define CX_CLASS()     ;

using namespace codex;

extern "C" {
CODEX_EXPORT bool             Reflect_DoesBehaviourExist(const char* className);
CODEX_EXPORT NativeBehaviour* Reflect_CreateBehaviour(const char* className, const Entity owner);
CODEX_EXPORT void             Dummy()
{
}
};

CX_CLASS()
class CODEX_EXPORT MienScripten : public NativeBehaviour
{
private:
    CX_SERIALIZE()
    bool m_Move = false;
    CX_SERIALIZE()
    f32 m_Multiplier = 1.0f;
    CX_SERIALIZE()
    Vector3f m_Axies = { 0.0f, 1.0f, 0.0f };

public:
    void   Init() override;
    void   Update(const f32 deltaTime) override;
    void   Serialize() override;
    object GetField(const std::string_view name) override;

    friend void                          to_json(nlohmann::ordered_json& json, const MienScripten& other);
    friend CODEX_EXPORT NativeBehaviour* Reflect_CreateBehaviour(const char* className, const Entity owner);
};
