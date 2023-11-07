#ifndef CODEX_CORE_LAYER_H
#define CODEX_CORE_LAYER_H

#include <sdafx.h>

#include "../Events/Event.h"

namespace codex {
    class Layer
    {
    protected:
        std::string m_DebugName;

    public:
        Layer(const std::string_view name = "Layer");
        virtual ~Layer() = default;

    public:
        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void Update(const f32 deltaTime) {}
        virtual void ImGuiRender() {}
        virtual void OnEvent(Event& event) {}

        const std::string& GetName() const noexcept { return m_DebugName; }
    };
} // namespace codex

#endif // CODEX_CORE_LAYER_H
