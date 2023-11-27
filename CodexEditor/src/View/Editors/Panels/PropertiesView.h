#pragma once

#include <Codex.h>

namespace codex::editor {
    // Forward declarations.
    struct SelectedEntityDescriptor;

    class PropertiesView
    {
    private:
        f32&                      m_ColumnWidth;
        SelectedEntityDescriptor& m_SelectedEntity;

    public:
        PropertiesView(f32& columnWidth, SelectedEntityDescriptor& selectedEntity);

    public:
        void OnImGuiRender();
    };
} // namespace codex::editor
