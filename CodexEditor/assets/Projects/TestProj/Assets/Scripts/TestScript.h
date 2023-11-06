#pragma once

#include <Modex.h>

using namespace codex;

class CODEX_API MienScripten : public NativeBehaviour
{
private:
	SERIALIZABLE()
	i32 m_MyInteger = 5;

public:
	void Init() override;
	void Update(const f32 deltaTime) override;
};
