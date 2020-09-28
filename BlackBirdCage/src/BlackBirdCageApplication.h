#pragma once

#include "BlackBirdBox.h"
#include "BlackBirdBox/Core/EntryPoint.h"
#include "BlackBirdCageLayer.h"

namespace BlackBirdCage {

class BlackBirdCageApplication : public BlackBirdBox::Application {
public:
    BlackBirdCageApplication();

    ~BlackBirdCageApplication() {}

    virtual void OnEvent(BlackBirdBox::Event& e) override;

private:
    bool OnKeyPressedEvent(BlackBirdBox::KeyPressedEvent& e);

private:
    BlackBirdBox::Ref<BlackBirdCageLayer> black_bird_cage_layer_;
};
}
