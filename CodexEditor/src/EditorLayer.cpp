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
        static auto& win       = Application::GetWindow();
        static SystemCursor cursor;
        static SystemCursor prev_cursor;
        static bool         is_dragging_window = false;

        // Window move
        // TODO: Replace the magic numbers.
        {
            static Vector2 diff{};

            auto pos       = Application::GetWindow().GetPosition();
            auto mouse_pos = Input::GetMousePos();
            if (Input::IsMouseDragging())
            {
                if (is_dragging_window)
                {
                    win.SetPosition(Input::GetScreenMousePos() - diff);
                    cursor = SystemCursor::Resize;
                }
                else if (mouse_pos.y >= 0 && mouse_pos.y <= 18)
                {
                    is_dragging_window = true;
                    diff       = Input::GetScreenMousePos() - win.GetPosition();
                }
            }
            else
            {
                is_dragging_window = false;
                cursor     = SystemCursor::Arrow;
            }
        }

        // Window resize
        {
            auto pos = Input::GetMousePos();
            static bool l = false, r = false;

            if (!is_dragging_window)
            {
                if (pos.y <= 3 || pos.y >= win.GetHeight() - 3)
                {
                    cursor = SystemCursor::VerticalResize;
                    l      = true;
                }
                else
                    l = false;
                if (pos.x <= 3 || pos.x >= win.GetWidth() - 3)
                {
                    cursor = SystemCursor::HorizontalResize;
                    r      = true;
                }
                else
                    r = false;

                if (l && r)
                {
                    cursor = SystemCursor::DiagonalLeftResize;
                }
            }
        }

        if (prev_cursor != cursor)
        {
            prev_cursor = cursor;
            win.SetCursor(cursor);
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
