#pragma once

#include <Codex.h>

using namespace codex;

class EditorLayer : public Layer
{
private:
    std::unique_ptr<Scene>  m_Scene          = nullptr;
    Entity                  m_Player         = Entity::None();
    ResRef<Shader>          m_BatchShader    = nullptr;
    std::unique_ptr<Camera> m_Camera         = nullptr;
    Entity                  m_SelectedEntity = Entity::None();

public:
    void OnAttach() override;
    void OnDetach() override;
    void Update(const f32 deltaTime) override;
    void ImGuiRender() override;

public:
    static void DrawVec3Control(const char* label, Vector3f& values, const f32 speed = 1.0f,
                                const f32 resetValue = 0.0f, const f32 columnWidth = 100.0f);
};
