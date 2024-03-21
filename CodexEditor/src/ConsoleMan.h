#pragma once

#include <Codex.h>

namespace codex::editor {
	class ConsoleMan : public Layer
	{
	private:
        static std::deque<std::string> m_Output;

    public:
        void OnAttach() override;
        void Update(const f32 deltaTime) override;
        void ImGuiRender() override;

	public:
		static void AppendMessage(const std::string_view msg) noexcept;
	};
} // namespace codex::editor