#pragma once

#include "BlackBirdBox.h"
#include "BlackBirdBox/Core/EntryPoint.h"
#include "ClothSimulationLayer.h"

namespace ClothSimulation {

class ClothSimulationApplication : public BlackBirdBox::Application {
public:
    ClothSimulationApplication();

    ~ClothSimulationApplication() {}

    virtual void OnEvent(BlackBirdBox::Event& e) override;

private:
    bool OnKeyPressedEvent(BlackBirdBox::KeyPressedEvent& e);

private:
    BlackBirdBox::Ref<ClothSimulationLayer> cloth_simulation_layer_;
};

}
