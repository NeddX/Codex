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
        DLib*                     m_ScriptModule;
        Vector4f&                 m_SelectColour;

    public:
        PropertiesView(f32& columnWidth, SelectedEntityDescriptor& selectedEntity, DLib* scriptModule,
                       Vector4f& selectColour);

    public:
        void OnImGuiRender();
    };
} // namespace codex::editor
