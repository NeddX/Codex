#pragma once

#include <Codex.h>

namespace codex::editor {
    // Forward declarations.
    struct SceneEditorDescriptor;

    class PropertiesView
    {
    private:
        mem::Ref<SceneEditorDescriptor> m_EditorDesc{};

    public:
        explicit PropertiesView(const mem::Ref<SceneEditorDescriptor>& editorDesc);

    public:
        void OnImGuiRender();
    };
} // namespace codex::editor
