#include "ConsoleMan.h"

namespace codex::editor {
    std::deque<std::string> ConsoleMan::m_Output;

    void ConsoleMan::OnAttach()
    {
    }

    void ConsoleMan::Update(const f32 deltaTime)
    {
    }

    void ConsoleMan::ImGuiRender()
    {
        ImGui::Begin("Console");
        const float footer_height = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
        if (ImGui::BeginChild("scroll_reg"))
        {
            for (const auto& e : m_Output)
                ImGui::Text("%s", e.c_str());
        }
        ImGui::EndChild();

        ImGui::End();
    }

    void ConsoleMan::AppendMessage(const std::string_view msg) noexcept
    {
        m_Output.emplace_back(msg);
    }

} // namespace codex::editor