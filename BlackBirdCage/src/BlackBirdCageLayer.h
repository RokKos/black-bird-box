#pragma once

#include "BlackBirdBox.h"

namespace BlackBirdCage {

class BlackBirdCageLayer : public BlackBirdBox::Layer {
public:
    BlackBirdCageLayer();
    virtual ~BlackBirdCageLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    void OnUpdate(BlackBirdBox::TimeStep ts) override;
    virtual void OnImGuiRender() override;
    void OnEvent(BlackBirdBox::Event& e) override;

private:
    BlackBirdBox::Ref<BlackBirdBox::FrameBuffer> frame_buffer_;

    bool viewport_focused_ = false, viewport_hovered_ = false;
    glm::vec2 viewport_size_ = { 0.0f, 0.0f };

    glm::vec4 bg_color_ = { 0.1f, 0.1f, 0.1f, 1 };

private:
};

}
