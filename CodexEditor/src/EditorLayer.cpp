#include "EditorLayer.h"

#include <ImGuizmo.h>
#include <tinyfiledialogs.h>

namespace codex::editor {
    namespace stdfs = std::filesystem;
    using namespace codex::events;
    using namespace codex::fs;
    using namespace codex::imgui;
    using namespace codex::mem;
    using namespace codex::graphics;
    using namespace codex::reflect;

    void EditorLayer::OnAttach()
    {
        m_SceneEditorView = new SceneEditorView;
        Application::Get().PushLayer(m_SceneEditorView);
    }

    void EditorLayer::OnDetach()
    {
    }

    void EditorLayer::Update(const f32 deltaTime)
    {
        
    }

    void EditorLayer::ImGuiRender()
    {
    }

    void EditorLayer::OnEvent(Event& e)
    {
        // EventDispatcher d(e);
        // d.Dispatch<KeyDownEvent>(BindEventDelegate(this, &EditorLayer::OnKeyDown_Event));
    }

    bool EditorLayer::OnKeyDown_Event(KeyDownEvent& e)
    {
        /*
        switch (e.GetKey())
        {
            case Key::Num1: m_GizmoMode = GizmoMode::Translation; return true;
            case Key::Num2: m_GizmoMode = GizmoMode::Rotation; return true;
            case Key::Num3: m_GizmoMode = GizmoMode::Scale; return true;
            default: break;
        }
        */
        return false;
    }
} // namespace codex::editor
