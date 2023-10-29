#ifndef CODEX_IMGUI_LAYER_H
#define CODEX_IMGUI_LAYER_H

#include <sdafx.h>

#include "../Core/Layer.h"
#include "../Events/Event.h"

namespace codex {
    class ImGuiLayer : public Layer
    {
    private:
        bool m_Blocking = true;

    public:
        ImGuiLayer();
        ~ImGuiLayer();

    public:
        inline void BlockEvents(const bool block) noexcept { m_Blocking = true; }

    public:
        void OnAttach() override;
        void OnDetach() override;
        void OnEvent(Event& event) override;

    public:
        void Begin();
        void End();
        void SetDarkThemeColours();
        u32  GetActiveWidgetID() const;
    };
} // namespace codex

#endif // CODEX_IMGUI_LAYER_H
