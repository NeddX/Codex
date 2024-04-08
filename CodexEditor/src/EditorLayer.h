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
        static ResRef<gfx::Shader>     m_BatchShader;
        static mem::Box<scene::Camera> m_Camera;

    public:
        static ResRef<gfx::Shader> GetBatchShader() noexcept;
        static scene::Camera&      GetCamera() noexcept;

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
