#include "TestScript.h"

void MienScripten::Init()
{
    std::cout << "Initialized baby!\n";
}

void MienScripten::Update(const f32 deltaTime)
{
    static float count     = 0;
    static auto& transform = GetComponent<TransformComponent>();
    transform.position.y += std::sin(count) * 100.0f * deltaTime;
    count += 0.1f;
}

void MienScripten::Serialize()
{
    m_SerializedData["MienScripten"]["Id"]                             = 0;
    m_SerializedData["MienScripten"]["Fields"]["m_MyInteger"]["Type"]  = "i32";
    m_SerializedData["MienScripten"]["Fields"]["m_MyInteger"]["Value"] = m_MyInteger;
}

void MienScripten2::Update(const f32 deltaTime)
{
    static float count     = 0;
    static auto& transform = GetComponent<TransformComponent>();
    transform.rotation.z += std::sin(count) * 100.0f * deltaTime;
    count += 0.1f;
}

void MienScripten2::Serialize()
{
}

bool Reflect_DoesBehaviourExist(const char* className)
{
    if (std::strcmp(className, "MienScripten") == 0)
        return true;
    else if (std::strcmp(className, "MienScripten2") == 0)
        return true;
    return false;
}

NativeBehaviour* Reflect_CreateBehaviour(const char* className, const Entity owner)
{
    std::printf("Creating instance of: %s\n", className);
    if (std::strcmp(className, "MienScripten") == 0)
    {
        auto* inst    = new MienScripten();
        inst->m_Owner = owner;
        return inst;
    }
    else if (std::strcmp(className, "MienScripten2") == 0)
    {
        auto* inst    = new MienScripten2();
        inst->m_Owner = owner;
        return inst;
    }
    else
        return nullptr;
}

object Reflect_GetField(const char* name, NativeBehaviour* bh)
{
}
