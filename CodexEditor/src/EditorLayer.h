#pragma once

#include <Codex.h>
#include <ImGuizmo.h>

#include "View/Editors/SceneEditorView.h"

namespace codex::editor {
    class EditorLayer : public Layer
    {
    private:
        SceneEditorView* m_SceneEditorView = nullptr;

    public:
        void OnAttach() override;
        void OnDetach() override;
        void Update(const f32 deltaTime) override;
        void ImGuiRender() override;

    public:
        void OnEvent(events::Event& e) override;
        bool OnKeyDown_Event(events::KeyDownEvent& e);
    };
} // namespace codex::editor
