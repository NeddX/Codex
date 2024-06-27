#pragma once

#include <Codex.h>

#include "EditorPanel.h"

namespace codex::editor {
    class SceneHierarchyView : public EditorPanel
    {
    public:
        using EditorPanel::EditorPanel;

    protected:
        void OnInit() override;
        void OnImGuiRender() override;
    };
} // namespace codex::editor
