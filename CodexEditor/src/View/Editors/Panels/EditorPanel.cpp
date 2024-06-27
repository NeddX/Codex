#include "EditorPanel.h"

#include "../SceneEditorView.h"

namespace codex::editor {
    EditorPanel::EditorPanel(SceneEditorView& view) noexcept
        : m_EditorView(view)
        , m_Desc(m_EditorView.m_Descriptor)
        , m_Show(true)
    {
    }

    EditorPanel::~EditorPanel() noexcept
    {
    }

    void EditorPanel::OnPreUpdate(const f32 deltaTime)
    {
        OnUpdate(deltaTime);

        // Update our panels.
        for (auto& panel : m_ViewPanels)
        {
            panel->OnPreUpdate(deltaTime);
        }
    }

    void EditorPanel::OnPreImGuiRender()
    {
        OnImGuiRender();

        auto block_events = GetImGuiBlockEvents();

        // Render our panels.
        {
            auto it = m_ViewPanels.begin();
            while (it != m_ViewPanels.end())
            {
                auto& panel = *it;
                if (!panel->m_Show)
                    it = m_ViewPanels.erase(it);
                else
                {
                    block_events = block_events && panel->GetImGuiBlockEvents();
                    panel->OnPreImGuiRender();
                    ++it;
                }
            }
        }

        SetImGuiBlockEvents(block_events);
    }

    void EditorPanel::OnPreEvent(events::Event& e)
    {
        // Child panels should be handled first because of painter's rule.
        for (auto& panel : m_ViewPanels)
        {
            if (e.handled)
                return;
            panel->OnPreEvent(e);
        }

        OnEvent(e);
    }
} // namespace codex::editor
