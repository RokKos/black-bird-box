#include "bbbpch.h"
#include "LayerStack.h"

namespace BlackBirdBox {

LayerStack::~LayerStack()
{
    PROFILE_FUNCTION();

    for (const Ref<Layer>& layer : layers_) {
        layer->OnDetach();
    }
}

void LayerStack::PushLayer(const Ref<Layer>& layer)
{
    PROFILE_FUNCTION();

    layers_.emplace(layers_.begin() + m_LayerInsertIndex, layer);
    m_LayerInsertIndex++;
}

void LayerStack::PushOverlay(const Ref<Layer>& overlay)
{
    PROFILE_FUNCTION();

    layers_.emplace_back(overlay);
}

void LayerStack::PopLayer(const Ref<Layer>& layer)
{
    PROFILE_FUNCTION();

    auto it = std::find(layers_.begin(), layers_.begin() + m_LayerInsertIndex, layer);
    if (it != layers_.begin() + m_LayerInsertIndex) {
        layer->OnDetach();
        layers_.erase(it);
        m_LayerInsertIndex--;
    }
}

void LayerStack::PopOverlay(const Ref<Layer>& overlay)
{
    PROFILE_FUNCTION();

    auto it = std::find(layers_.begin() + m_LayerInsertIndex, layers_.end(), overlay);
    if (it != layers_.end()) {
        overlay->OnDetach();
        layers_.erase(it);
    }
}

}
