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

    const std::string& GetGeneralSettingFilePath() const { return general_settings_file_path_; }
    const std::string& GetSimulationSettingFilePath() const { return simulation_settings_file_path_; }

private:
    bool OnKeyPressedEvent(BlackBirdBox::KeyPressedEvent& e);

private:
    std::string general_settings_file_path_ = "assets/";
    std::string simulation_settings_file_path_;
    BlackBirdBox::Ref<ClothSimulationLayer> cloth_simulation_layer_;
};

}
