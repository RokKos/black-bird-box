#include "ClothSimulationApplication.h"

namespace ClothSimulation {

ClothSimulationApplication::ClothSimulationApplication()
{
    cloth_simulation_layer_ = BlackBirdBox::CreateRef<ClothSimulationLayer>();
    PushLayer(cloth_simulation_layer_);
}

void ClothSimulationApplication::OnEvent(BlackBirdBox::Event& e)
{
    Application::OnEvent(e);

    PROFILE_FUNCTION();

    BlackBirdBox::EventDispatcher dispatcher(e);
    dispatcher.Dispatch<BlackBirdBox::KeyPressedEvent>(BIND_EVENT_FN(ClothSimulationApplication::OnKeyPressedEvent));
}

bool ClothSimulationApplication::OnKeyPressedEvent(BlackBirdBox::KeyPressedEvent& e)
{
    PROFILE_FUNCTION();

    if (e.GetKeyCode() == BlackBirdBox::KeyCode::F6) {
        PopLayer(cloth_simulation_layer_);
        cloth_simulation_layer_ = BlackBirdBox::CreateRef<ClothSimulationLayer>();
        PushLayer(cloth_simulation_layer_);
    }

    return false;
}

}

namespace BlackBirdBox {
Application* CreateApplication() { return new ClothSimulation::ClothSimulationApplication(); }
}
