#ifndef CODEX_CORE_LAYER_STACK_H
#define CODEX_CORE_LAYER_STACK_H

#include <sdafx.h>

#include "Layer.h"

namespace codex {
    class CODEX_API LayerStack
    {
    private:
        std::vector<Layer*> m_Layers;
        u32                 m_LayerPointer = 0;

    public:
        LayerStack() = default;
        ~LayerStack();

    public:
        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* overlay);

    public:
        std::vector<Layer*>::iterator               begin() noexcept { return m_Layers.begin(); }
        std::vector<Layer*>::iterator               end() noexcept { return m_Layers.end(); }
        std::vector<Layer*>::reverse_iterator       rbegin() noexcept { return m_Layers.rbegin(); }
        std::vector<Layer*>::reverse_iterator       rend() noexcept { return m_Layers.rend(); }
        std::vector<Layer*>::const_iterator         begin() const noexcept { return m_Layers.begin(); }
        std::vector<Layer*>::const_iterator         end() const noexcept { return m_Layers.end(); }
        std::vector<Layer*>::const_reverse_iterator rbegin() const noexcept { return m_Layers.rbegin(); }
        std::vector<Layer*>::const_reverse_iterator rend() const noexcept { return m_Layers.rend(); }
    };
} // namespace codex

#endif // CODEX_CORE_LAYER_STACK_H
