#pragma once

#include <Codex.h>

namespace codex::editor {
    // Forward declarations.
    struct SelectedEntityDescriptor;

    class SceneHierarchyView
    {
    private:
        Scene&                    m_Scene;
        SelectedEntityDescriptor& m_SelectedEntity;
        Vector4f&                 m_SelectColour;

    public:
        SceneHierarchyView(Scene& scene, SelectedEntityDescriptor& selectedEntity, Vector4f& m_SelectColour);

        void OnImGuiRender();
    };
} // namespace codex::editor
