#include "TestScript.h"

void MienScripten::Init()
{
    std::cout << "Initialized baby!\n";
}

void MienScripten::Update(const f32 deltaTime)
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

void MienScripten::Serialize()
{
    m_SerializedData["MienScripten"]["Id"]                              = 0;
    m_SerializedData["MienScripten"]["Fields"]["m_Multiplier"]["Type"]  = FieldType::F32;
    m_SerializedData["MienScripten"]["Fields"]["m_Multiplier"]["Value"] = m_Multiplier;
    m_SerializedData["MienScripten"]["Fields"]["m_Move"]["Type"]        = FieldType::Boolean;
    m_SerializedData["MienScripten"]["Fields"]["m_Move"]["Value"]       = m_Move;
    m_SerializedData["MienScripten"]["Fields"]["m_Axies"]["Type"]       = FieldType::Vector3f;
    m_SerializedData["MienScripten"]["Fields"]["m_Axies"]["Value"]      = m_Axies;
}

object MienScripten::GetField(const std::string_view name)
{
    if (std::strncmp(name.data(), "m_Multiplier", name.size()) == 0)
        return &m_Multiplier;
    else if (std::strncmp(name.data(), "m_Move", name.size()) == 0)
        return &m_Move;
    else if (std::strncmp(name.data(), "m_Axies", name.size()) == 0)
        return &m_Axies;
    return nullobj;
}

bool Reflect_DoesBehaviourExist(const char* className)
{
    if (std::strcmp(className, "MienScripten") == 0)
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
    else
        return nullptr;
}
