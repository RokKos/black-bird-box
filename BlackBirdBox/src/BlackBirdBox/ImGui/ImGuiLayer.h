#pragma once

#include "BlackBirdBox/Layer/Layer.h"

#include "BlackBirdBox/Events/ApplicationEvent.h"
#include "BlackBirdBox/Events/KeyEvent.h"
#include "BlackBirdBox/Events/MouseEvent.h"

namespace BlackBirdBox {

class ImGuiLayer : public Layer {
public:
    ImGuiLayer();
    ~ImGuiLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnEvent(Event& e) override;

    void Begin();
    void End();

    void SetBlocksEvents(bool blocks_events) { blocks_events_ = blocks_events; };

private:
    float m_Time = 0.0f;
    bool blocks_events_ = false;
};

}
