#pragma once

#include <Codex.h>
#include <ImGuizmo.h>

using namespace codex;

struct SelectedEntityDescriptor
{
    Entity   entity = Entity::None();
    Vector4f overlayColour{ 0.0f, 0.0f, 0.0f, 0.0f };
};

enum class GizmoMode
{
    Translation = ImGuizmo::OPERATION::TRANSLATE,
    Rotation    = ImGuizmo::OPERATION::ROTATE,
    Scale       = ImGuizmo::OPERATION::SCALE
};

class EditorLayer : public Layer
{
private:
    std::unique_ptr<Scene>            m_Scene       = nullptr;
    ResRef<Shader>                    m_BatchShader = nullptr;
    std::unique_ptr<Camera>           m_Camera      = nullptr;
    f32                               m_ColumnWidth = 120.0f;
    std::unique_ptr<mgl::FrameBuffer> m_Framebuffer = nullptr;
    Vector2f                          m_ViewportBounds[2];
    SelectedEntityDescriptor          m_SelectedEntity;
    Vector4f                          m_SelectColour = { 0.5f, 1.0f, 0.5f, 1.0f };
    bool                              m_GizmoActive  = false;
    GizmoMode                         m_GizmoMode    = GizmoMode::Translation;
    std::filesystem::path m_ProjectPath;
    NativeBehaviour* m_Script = nullptr;
    std::unique_ptr<DLib> m_ScriptModule = nullptr;

public:
    void OnAttach() override;
    void OnDetach() override;
    void Update(const f32 deltaTime) override;
    void ImGuiRender() override;

public:
    void OnEvent(Event& e) override;
    bool OnKeyDown_Event(KeyDownEvent& e);

public:
    static void DrawVec3Control(const char* label, Vector3f& values, const f32 columnWidth = 100.0f,
                                const f32 speed = 1.0f, const f32 resetValue = 0.0f);
    static void DrawVec2Control(const char* label, Vector2f& values, const f32 columnWidth = 100.0,
                                const f32 speed = 1.0f, const f32 resetValue = 0.0);
};
