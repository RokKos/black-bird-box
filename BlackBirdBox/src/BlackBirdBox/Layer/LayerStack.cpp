#include "bbbpch.h"
#include "LayerStack.h"

namespace BlackBirdBox {

LayerStack::~LayerStack()
{
    PROFILE_FUNCTION();

    for (Layer* layer : m_Layers) {
        layer->OnDetach();
        delete layer;
    }
}

void LayerStack::PushLayer(Layer* layer)
{
    PROFILE_FUNCTION();

    m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
    m_LayerInsertIndex++;
}

void LayerStack::PushOverlay(Layer* overlay)
{
    PROFILE_FUNCTION();

    m_Layers.emplace_back(overlay);
}

void LayerStack::PopLayer(Layer* layer)
{
    PROFILE_FUNCTION();

    auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
    if (it != m_Layers.begin() + m_LayerInsertIndex) {
        layer->OnDetach();
        m_Layers.erase(it);
        m_LayerInsertIndex--;
    }
}

void LayerStack::PopOverlay(Layer* overlay)
{
    PROFILE_FUNCTION();

    auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), overlay);
    if (it != m_Layers.end()) {
        overlay->OnDetach();
        m_Layers.erase(it);
    }
}

}
