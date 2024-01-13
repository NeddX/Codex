#pragma once

#include <Codex.h>
#include <ImGuizmo.h>

#include "View/Editors/SceneEditorView.h"

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
    mem::Box<Scene>                   m_Scene       = nullptr;
    ResRef<graphics::Shader>          m_BatchShader = nullptr;
    mem::Box<Camera>                  m_Camera      = nullptr;
    f32                               m_ColumnWidth = 120.0f;
    mem::Box<mgl::FrameBuffer>        m_Framebuffer = nullptr;
    Vector2f                          m_ViewportBounds[2];
    SelectedEntityDescriptor          m_SelectedEntity;
    Vector4f                          m_SelectColour = { 0.5f, 1.0f, 0.5f, 1.0f };
    bool                              m_GizmoActive  = false;
    GizmoMode                         m_GizmoMode    = GizmoMode::Translation;
    std::filesystem::path             m_ProjectPath{};
    NativeBehaviour*                  m_Script          = nullptr;
    mem::Box<DLib>                    m_ScriptModule    = nullptr;
    mem::Box<editor::SceneEditorView> m_SceneEditorView = nullptr;

public:
    void OnAttach() override;
    void OnDetach() override;
    void Update(const f32 deltaTime) override;
    void ImGuiRender() override;

public:
    void OnEvent(events::Event& e) override;
    bool OnKeyDown_Event(events::KeyDownEvent& e);
};
