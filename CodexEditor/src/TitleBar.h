#pragma once

#include <Codex.h>
#include <array>

namespace codex::editor {
    class TitleBar : public Layer
    {
    private:
        f32                           m_TitleBarHeight = 30.0f;
        std::array<gfx::Texture2D, 3> m_TitleBarIcons;
        bool                          m_ResizeTriggered = false;

    public:
        TitleBar();
        ~TitleBar();

    protected:
        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(const f32 deltaTime) override;
        void OnImGuiRender() override;
        void DrawTitleBar();
        void OnEvent(events::Event& event) override;
    };
} // namespace codex::editor
