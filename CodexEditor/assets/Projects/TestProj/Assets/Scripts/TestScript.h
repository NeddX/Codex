#pragma once

#include <Modex.h>

using namespace codex;

class CODEX_EXPORT MienScripten : public NativeBehaviour
{
private:
    i32 m_MyInteger = 5;

public:
    void Init() override;
    void Update(const f32 deltaTime) override;

    friend void to_json(nlohmann::ordered_json& json, const MienScripten& other);
};

//CODEX_EXPORT void            to_json(nlohmann::ordered_json& j, const MienScripten& other);
extern "C" {
    CODEX_EXPORT bool Reflect_DoesBehaviourExist(const char* className);
    CODEX_EXPORT bool Reflect_SerializeBehaviour(const char* className, nlohmann::ordered_json* j, const NativeBehaviour* obj);
    CODEX_EXPORT NativeBehaviour* Reflect_CreateBehaviour(const char* className);
    CODEX_EXPORT void Dummy() {}
};