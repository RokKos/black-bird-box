#include "BlackBirdCageApplication.h"

namespace BlackBirdCage {

BlackBirdCageApplication::BlackBirdCageApplication()
{
    black_bird_cage_layer_ = BlackBirdBox::CreateRef<BlackBirdCageLayer>();
    PushLayer(black_bird_cage_layer_);
}

void BlackBirdCageApplication::OnEvent(BlackBirdBox::Event& e)
{
    Application::OnEvent(e);

    PROFILE_FUNCTION();

    BlackBirdBox::EventDispatcher dispatcher(e);
    dispatcher.Dispatch<BlackBirdBox::KeyPressedEvent>(BIND_EVENT_FN(BlackBirdCageApplication::OnKeyPressedEvent));
}

bool BlackBirdCageApplication::OnKeyPressedEvent(BlackBirdBox::KeyPressedEvent& e)
{
    PROFILE_FUNCTION();

    return false;
}
}

namespace BlackBirdBox {
Application* CreateApplication() { return new BlackBirdCage::BlackBirdCageApplication(); }
}
