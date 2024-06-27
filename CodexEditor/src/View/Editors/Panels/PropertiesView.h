#pragma once

#include <Codex.h>

#include "EditorPanel.h"

namespace codex::editor {
    class PropertiesView : public EditorPanel
    {
    public:
        using EditorPanel::EditorPanel;

    protected:
        void OnInit() override;
        void OnImGuiRender() override;

    private:
        static void DrawPhysicsMaterial2DControl(phys::PhysicsMaterial2D& mat, const f32 columnWidth) noexcept;
    };
} // namespace codex::editor
