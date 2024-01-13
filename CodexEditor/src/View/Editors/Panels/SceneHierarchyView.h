#pragma once

#include <Codex.h>

namespace codex::editor {
    // Forward declarations.
    struct SceneEditorDescriptor;

    class SceneHierarchyView
    {
    private:
        mem::Ref<SceneEditorDescriptor> m_EditorDesc{};

    public:
        explicit SceneHierarchyView(const mem::Ref<SceneEditorDescriptor>& editorDesc);

        void OnImGuiRender();
    };
} // namespace codex::editor
