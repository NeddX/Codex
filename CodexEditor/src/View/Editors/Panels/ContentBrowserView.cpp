#include "ContentBrowserView.h"

namespace codex::editor {
    void ContentBrowserView::OnInit()
    {
        auto& desc = GetDescriptor().Lock();
    }

    void ContentBrowserView::OnImGuiRender()
    {
        ImGui::Begin("Content Browser");
        ImGui::End();
    }
} // namespace codex::editor
