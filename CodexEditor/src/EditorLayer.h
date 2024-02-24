#pragma once

#include <Codex.h>
#include <ImGuizmo.h>

#include "View/Editors/SceneEditorView.h"

namespace codex::editor {
    class EditorLayer : public Layer
    {
    private:
        mem::Box<SceneEditorView> m_SceneEditorView = nullptr;

    private:
        static ResRef<graphics::Shader> m_BatchShader;
        static mem::Box<Camera>         m_Camera;

    public:
        static ResRef<graphics::Shader> GetBatchShader() noexcept;
        static Camera&                  GetCamera() noexcept;

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
