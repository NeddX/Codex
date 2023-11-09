#include "TestScript.h"

void MienScripten::Init()
{
    std::cout << "Initialized baby!\n";
}

void MienScripten::Update(const f32 deltaTime)
{
    std::cout << "more stuff" << std::endl;
}

void to_json(nlohmann::ordered_json& j, const MienScripten& other)
{
	j["MienScripten"]["Id"] = 0;
	j["MienScripten"]["Fields"]["m_MyInteger"]["Type"] = "i32";
	j["MienScripten"]["Fields"]["m_MyInteger"]["Value"] = other.m_MyInteger;
}

extern "C" {
	CODEX_EXPORT bool Reflect_DoesBehaviourExist(const char* className)
	{
		if (std::strcmp(className, "MienScripten") == 0)
			return true;
		return false;
	}



	CODEX_EXPORT bool Reflect_SerializeBehaviour(const char* className, nlohmann::ordered_json* j, const NativeBehaviour* obj)
	{
		if (std::strcmp(className, "MienScripten") == 0)
		{
			to_json(*j, (MienScripten&)(*obj));
			return true;
		}
		return false;
	}

	CODEX_EXPORT NativeBehaviour* Reflect_CreateBehaviour(const char* className)
	{
		std::printf("Creating instance of: %s\n", className);
		if (std::strcmp(className, "MienScripten") == 0)
			return new MienScripten();
		else
			return nullptr;
	}
};
