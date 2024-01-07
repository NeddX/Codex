#pragma once

#include <Codex.h>

namespace codex::editor {
    // Forward declarations.
    struct SceneEditorDescriptor;

    class PropertiesView
    {
    private:
        Ref<SceneEditorDescriptor> m_EditorDesc{};

    public:
        explicit PropertiesView(const Ref<SceneEditorDescriptor>& editorDesc);

    public:
        void OnImGuiRender();
    };
} // namespace codex::editor
