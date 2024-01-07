#pragma once

#include <Codex.h>

namespace codex::editor {
    // Forward declarations.
    struct SceneEditorDescriptor;

    class SceneHierarchyView
    {
    private:
        Ref<SceneEditorDescriptor> m_EditorDesc{};

    public:
        explicit SceneHierarchyView(const Ref<SceneEditorDescriptor>& editorDesc);

        void OnImGuiRender();
    };
} // namespace codex::editor
