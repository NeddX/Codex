#pragma once

#include <Codex.h>
#include <array>

namespace codex::editor {
    class Icon
    {
    private:
        gfx::Texture2D        m_Texture;
        mem::Box<mgl::FrameBuffer> m_Fb;
        Matrix4f                   m_TransformMat;

    public:
        Icon() = default;
        Icon(gfx::Texture2D texture, const mgl::FrameBufferProperties props);

    public:
        inline u32 GetGLId() const noexcept { return m_Texture.GetGlId(); }

    public:
        void Render();
    };

    class TitleBar : public Layer
    {
    private:
        f32                 m_TitleBarHeight = 30.0f;
        std::array<Icon, 3> m_TitleBarIcons;
        bool                m_ResizeTriggered = false;

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