#pragma once

#include "BlackBirdBox/Core/Core.h"
#include "Layer.h"

namespace BlackBirdBox {

class LayerStack {
public:
    LayerStack() = default;
    ~LayerStack();

    void PushLayer(const Ref<Layer>& layer);
    void PushOverlay(const Ref<Layer>& overlay);
    void PopLayer(const Ref<Layer>& layer);
    void PopOverlay(const Ref<Layer>& overlay);

    std::vector<Ref<Layer>>::iterator begin() { return layers_.begin(); }
    std::vector<Ref<Layer>>::iterator end() { return layers_.end(); }
    std::vector<Ref<Layer>>::reverse_iterator rbegin() { return layers_.rbegin(); }
    std::vector<Ref<Layer>>::reverse_iterator rend() { return layers_.rend(); }

    std::vector<Ref<Layer>>::const_iterator begin() const { return layers_.begin(); }
    std::vector<Ref<Layer>>::const_iterator end() const { return layers_.end(); }
    std::vector<Ref<Layer>>::const_reverse_iterator rbegin() const { return layers_.rbegin(); }
    std::vector<Ref<Layer>>::const_reverse_iterator rend() const { return layers_.rend(); }

private:
    std::vector<Ref<Layer>> layers_;
    unsigned int m_LayerInsertIndex = 0;
};

}
