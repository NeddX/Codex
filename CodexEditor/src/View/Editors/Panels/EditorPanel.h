#pragma once

#include <Codex.h>

#include "../SceneEditorView.h"

namespace codex::editor {
    class EditorPanel
    {
        friend class SceneEditorView;

    private:
        SceneEditorView&                   m_EditorView;
        mem::Ref<SceneEditorDescriptor>    m_Desc;
        std::vector<mem::Box<EditorPanel>> m_ViewPanels;
        bool                               m_ImGuiBlockEvents = true;

    protected:
        bool m_Show;
        bool m_Focus = false;

    public:
        explicit EditorPanel(SceneEditorView& view) noexcept;
        virtual ~EditorPanel() noexcept;

    public:
        CX_PROPERTY(ImGuiBlockEvents)
        [[nodiscard]] inline mem::Ref<SceneEditorDescriptor> GetDescriptor() const noexcept { return m_Desc; }
        [[nodiscard]] inline SceneEditorView&                GetParent() noexcept { return m_EditorView; }
        [[nodiscard]] inline const SceneEditorView&          GetParent() const noexcept
        {
            return const_cast<EditorPanel*>(this)->GetParent();
        }
            inline void Focus() noexcept { m_Focus = true; }
        inline void Close() noexcept { m_Show = false; }

    protected:
        template <typename T>
            requires(std::is_base_of_v<EditorPanel, T>)
        T& AttachPanel() noexcept
        {
            for (auto& e : m_ViewPanels)
            {
                if (auto* ptr = dynamic_cast<T*>(e.Get()); ptr != nullptr)
                    return *ptr;
            }

            m_ViewPanels.emplace_back(new T(GetParent()));
            m_ViewPanels.back()->OnInit();
            return *static_cast<T*>(m_ViewPanels.back().Get());
        }
        template <typename T>
            requires(std::is_base_of_v<EditorPanel, T>)
        void DetachPanel() noexcept
        {
            for (auto it = m_ViewPanels.begin(); it != m_ViewPanels.end(); ++it)
            {
                if (dynamic_cast<T*>(it->Get()) != nullptr)
                {
                    m_ViewPanels.erase(it);
                    break;
                }
            }
        }

    private:
        void         OnPreUpdate(const f32 deltaTime);
        virtual void OnImGuiRender() = 0;
        void         OnPreEvent(events::Event& e);

    protected:
        virtual void OnInit() = 0;
        virtual void OnUpdate(const f32 deltaTime) {};
        void         OnPreImGuiRender();
        virtual void OnEvent(events::Event& e) {}
    };
} // namespace codex::editor
