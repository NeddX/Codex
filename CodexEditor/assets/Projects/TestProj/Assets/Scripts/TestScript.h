#pragma once

#include <Modex.h>

using namespace codex;

class CODEX_API MienScripten : public NativeBehaviour
{
private:
	i32 m_MyInteger = 5;

public:
	void Init() override;
	void Update(const f32 deltaTime) override;

	friend void to_json(nlohmann::ordered_json& json, const MienScripten& other);
};

void to_json(nlohmann::ordered_json& json, const MienScripten& other);
