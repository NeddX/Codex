#pragma once

#include <Codex.h>
#include <ImGuizmo.h>

#include "View/Editors/SceneEditorView.h"

namespace codex::editor {
    class Editor : public Layer
    {
    private:
        mem::Box<SceneEditorView> m_SceneEditorView = nullptr;

    private:
        static std::optional<scene::EditorCamera> s_Camera;

    public:
        [[nodiscard]] static inline scene::EditorCamera& GetViewportCamera() noexcept { return *s_Camera; }

    public:
        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(const f32 deltaTime) override;
        void OnImGuiRender() override;

    public:
        void OnEvent(events::Event& e) override;
        bool OnKeyDown_Event(events::KeyDownEvent& e);
    };
} // namespace codex::editor
