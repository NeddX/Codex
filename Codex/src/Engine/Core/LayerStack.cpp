#include "LayerStack.h"

namespace codex {
    LayerStack::~LayerStack()
    {
        for (auto layer : m_Layers)
        {
            layer->OnDetach();
            delete layer;
        }
    }

    void LayerStack::PushLayer(Layer* layer)
    {
        m_Layers.emplace(m_Layers.begin() + m_LayerPointer++, layer);
    }

    void LayerStack::PushOverlay(Layer* overlay)
    {
        m_Layers.push_back(overlay);
    }

    void LayerStack::PopLayer(Layer* layer)
    {
        auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerPointer, layer);
        if (it != m_Layers.begin() + m_LayerPointer)
        {
            layer->OnDetach();
            m_Layers.erase(it);
            --m_LayerPointer;
        }
    }

    void LayerStack::PopOverlay(Layer* overlay)
    {
        auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerPointer, overlay);
        if (it != m_Layers.end())
        {
            overlay->OnDetach();
            m_Layers.erase(it);
        }
    }
} // namespace codex
