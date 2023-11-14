#pragma once

#include <Modex.h>

using namespace codex;

extern "C" {
CODEX_EXPORT bool             Reflect_DoesBehaviourExist(const char* className);
CODEX_EXPORT NativeBehaviour* Reflect_CreateBehaviour(const char* className, const Entity owner);
CODEX_EXPORT object           Reflect_GetField(const char* name, const NativeBehaviour* behaviour);
CODEX_EXPORT void             Dummy()
{
}
};

class CODEX_EXPORT MienScripten : public NativeBehaviour
{
private:
    i32 m_MyInteger = 5;

public:
    void Init() override;
    void Update(const f32 deltaTime) override;
    void Serialize() override;

    friend void                          to_json(nlohmann::ordered_json& json, const MienScripten& other);
    friend CODEX_EXPORT NativeBehaviour* Reflect_CreateBehaviour(const char* className, const Entity owner);
    friend CODEX_EXPORT object           Reflect_GetField(const char* name, NativeBehaviour* bh);
};

class CODEX_EXPORT MienScripten2 : public NativeBehaviour
{
public:
    void Update(const f32 deltaTime) override;
    void Serialize() override;

    friend void                          to_json(nlohmann::ordered_json& json, const MienScripten& other);
    friend CODEX_EXPORT NativeBehaviour* Reflect_CreateBehaviour(const char* className, const Entity owner);
    friend CODEX_EXPORT object           Reflect_GetField(const char* name, NativeBehaviour* bh);
};
