#pragma once

#include <Codex.h>

#include "EditorPanel.h"

namespace codex::editor {
    class ToolbarView : public EditorPanel
    {
    public:
        using EditorPanel::EditorPanel;

    private:
        gfx::Texture2D m_PlayIcon;
        gfx::Texture2D m_StopIcon;
        gfx::Texture2D m_SimulateIcon;

    protected:
        void OnInit() override;
        void OnImGuiRender() override;
    };
} // namespace codex::editor
