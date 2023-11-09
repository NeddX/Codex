#include "TestScript.h"

void MienScripten::Init()
{
    std::cout << "Initialized baby!\n";
}

void MienScripten::Update(const f32 deltaTime)
{
	static float count = 0;
	static auto& transform = GetComponent<TransformComponent>();
	transform.position.y += std::sin(count) * 100.0f * deltaTime;
	count += 0.1f;
}

void MienScripten2::Update(const f32 deltaTime)
{
	static float count = 0;
	static auto& transform = GetComponent<TransformComponent>();
	transform.rotation.z += std::sin(count) * 100.0f * deltaTime;
	count += 0.1f;
}

void to_json(nlohmann::ordered_json& j, const MienScripten& other)
{
	j["MienScripten"]["Id"] = 0;
	j["MienScripten"]["Fields"]["m_MyInteger"]["Type"] = "i32";
	j["MienScripten"]["Fields"]["m_MyInteger"]["Value"] = other.m_MyInteger;
}

bool Reflect_DoesBehaviourExist(const char* className)
{
	if (std::strcmp(className, "MienScripten") == 0)
		return true;
	else if (std::strcmp(className, "MienScripten2") == 0)
		return true;
	return false;
}

bool Reflect_SerializeBehaviour(const char* className, nlohmann::ordered_json* j, const NativeBehaviour* obj)
{
	if (std::strcmp(className, "MienScripten") == 0)
	{
		to_json(*j, (MienScripten&)(*obj));
		return true;
	}
	return false;
}

NativeBehaviour* Reflect_CreateBehaviour(const char* className, const Entity owner)
{
	std::printf("Creating instance of: %s\n", className);
	if (std::strcmp(className, "MienScripten") == 0)
	{
		auto* inst = new MienScripten();
		inst->m_Owner = owner;
		return inst;
	}
	else if (std::strcmp(className, "MienScripten2") == 0)
	{
		auto* inst = new MienScripten2();
		inst->m_Owner = owner;
		return inst;
	}
	else
		return nullptr;
}