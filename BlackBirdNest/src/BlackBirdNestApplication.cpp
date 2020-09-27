#include "BlackBirdNestApplication.h"

namespace BlackBirdNest {

BlackBirdNestApplication::BlackBirdNestApplication()
{
    black_bird_nest_layer_ = BlackBirdBox::CreateRef<BlackBirdNestLayer>();
    PushLayer(black_bird_nest_layer_);
}

void BlackBirdNestApplication::OnEvent(BlackBirdBox::Event& e)
{
    Application::OnEvent(e);

    PROFILE_FUNCTION();

    BlackBirdBox::EventDispatcher dispatcher(e);
    dispatcher.Dispatch<BlackBirdBox::KeyPressedEvent>(BIND_EVENT_FN(BlackBirdNestApplication::OnKeyPressedEvent));
}

bool BlackBirdNestApplication::OnKeyPressedEvent(BlackBirdBox::KeyPressedEvent& e)
{
    PROFILE_FUNCTION();

    if (e.GetKeyCode() == BlackBirdBox::KeyCode::F6) {
        PopLayer(black_bird_nest_layer_);
        black_bird_nest_layer_ = BlackBirdBox::CreateRef<BlackBirdNestLayer>();
        PushLayer(black_bird_nest_layer_);
    }

    return false;
}
}

namespace BlackBirdBox {
Application* CreateApplication() { return new BlackBirdNest::BlackBirdNestApplication(); }
}
