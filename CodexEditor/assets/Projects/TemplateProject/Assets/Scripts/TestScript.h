#pragma once

#include <Codex.h>

#include <string>

using namespace codex;

RF_CLASS()
class CODEX_EXPORT MienScripten : public NativeBehaviour
{
    RF_GENERATE_BODY()

private:
	RF_SERIALIZABLE()
	std::string m_Str;
    RF_SERIALIZABLE()
    bool m_Move = true;
    RF_SERIALIZABLE()
    f32 m_Multiplier = 1.0f;
    RF_SERIALIZABLE()
    Vector3f m_Axies = { 0.0f, 1.0f, 0.0f };

public:
    void OnInit() override;
	void OnDispose() override;
    void OnUpdate(const f32 deltaTime) override;
};

RF_CLASS()
class CODEX_EXPORT RotateScript : public NativeBehaviour
{
    RF_GENERATE_BODY()
	
public:
	RF_SERIALIZABLE()
	bool m_Move = false;
	RF_SERIALIZABLE()
	f32 m_Speed = 10.0f;

public:
    void OnInit() override {}
    void OnUpdate(const f32 deltaTime) override 
	{ 
		if (m_Move) 
		{ 
			GetComponent<TransformComponent>().rotation.z += m_Speed * deltaTime; 
		}
	}
};
