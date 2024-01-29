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
        auto pos       = Application::GetWindow().GetPosition();
        auto mouse_pos = Input::GetMousePos();

        // Window move
        // TODO: Replace the magic numbers.
        {
            auto& win = Application::GetWindow();
                static Vector2 diff{};
                static bool    drag_valid = false;
                if (Input::IsMouseDragging())
                {
                    if (drag_valid)
                        win.SetPosition(Input::GetScreenMousePos() - diff);
                    else if (mouse_pos.y >= 0 && mouse_pos.y <= 18)
                    {
                        drag_valid = true;
                        diff       = Input::GetScreenMousePos() - Application::GetWindow().GetPosition();
                    }
                }
                else
                    drag_valid = false;
        }

        // Window resize
        {
            auto& win = Application::GetWindow();
            auto pos = Input::GetMousePos();
            if ((pos.x >= 0 && pos.x <= 3) || (pos.x >= win.GetWidth() - 3 && pos.x <= win.GetWidth()))
            {
                win.SetCursor(SystemCursor::DiagonalLeftResize);
            }
            else
            {
                win.SetCursor(SystemCursor::Arrow);
            }
        }
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
