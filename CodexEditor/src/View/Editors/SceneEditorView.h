#pragma once

#include <Codex.h>
#include <ImGuizmo.h>

#include "Panels/PropertiesView.h"
#include "Panels/SceneHierarchyView.h"

namespace codex::editor {
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

    struct SceneEditorDescriptor
    {
        mem::Box<Scene>          scene{};
        mem::Box<DLib>           scriptModule{};
        SelectedEntityDescriptor selectedEntity{};
        f32                      columnWidth  = 120.0f;
        Vector4f                 selectColour = { 0.5f, 1.0f, 0.5f, 1.0f };
        std::filesystem::path    currentProjectPath{};
    };

    class SceneEditorView : public Layer
    {
    private:
        ResRef<graphics::Shader>           m_BatchShader = nullptr;
        mem::Box<Camera>                   m_Camera      = nullptr;
        mem::Box<mgl::FrameBuffer>         m_Framebuffer = nullptr;
        Vector2f                           m_ViewportBounds[2]{};
        bool                               m_GizmoActive = false;
        GizmoMode                          m_GizmoMode   = GizmoMode::Translation;
        NativeBehaviour*                   m_Script      = nullptr;
        mem::Shared<SceneEditorDescriptor> m_Descriptor  = nullptr;
        std::filesystem::path              m_ApplicationDataPath{};
        std::filesystem::path              m_VariableApplicationDataPath{};

    private:
        // Panels
        mem::Box<SceneHierarchyView> m_SceneHierarchyView = nullptr;
        mem::Box<PropertiesView>     m_PropertiesView     = nullptr;

    public:
        SceneEditorView() = default;

    public:
        void OnAttach() override;
        void OnDetach() override;
        void Update(const f32 deltaTime) override;
        void ImGuiRender() override;

    public:
        void OnEvent(events::Event& e) override;
        bool OnKeyDown_Event(events::KeyDownEvent& e);

    public:
        void LoadScriptModule();
        void UnloadScriptModule();

    public:
        static void DrawVec3Control(const char* label, Vector3f& values, const f32 columnWidth = 100.0f,
                                    const f32 speed = 1.0f, const f32 resetValue = 0.0f);
        static void DrawVec2Control(const char* label, Vector2f& values, const f32 columnWidth = 100.0,
                                    const f32 speed = 1.0f, const f32 resetValue = 0.0);
    };
} // namespace codex::editor
