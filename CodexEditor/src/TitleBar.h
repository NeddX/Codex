#pragma once

#include <Codex.h>

namespace codex::editor {
	class TitleBar : public codex::Layer
    {
    private:
        f32 m_TitleBarHeight = 30.0f;
        u8* m_CloseSymbol    = nullptr;
        u8* m_MaximizeSymbol = nullptr;
        u8* m_MinimizeSymbol = nullptr;

    public:
        TitleBar();
        ~TitleBar();

    protected:
        void OnAttach() override;
        void OnDetach() override;
        void Update(const codex::f32 deltaTime) override;
        void ImGuiRender() override;
        void DrawTitleBar();
        void OnEvent(events::Event& event) override;
	};
} // namespace codex