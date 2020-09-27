#pragma once

#include "BlackBirdBox.h"
#include "BlackBirdBox/Core/EntryPoint.h"
#include "BlackBirdNestLayer.h"

namespace BlackBirdNest {

class BlackBirdNestApplication : public BlackBirdBox::Application {
public:
    BlackBirdNestApplication();

    ~BlackBirdNestApplication() {}

    virtual void OnEvent(BlackBirdBox::Event& e) override;

private:
    bool OnKeyPressedEvent(BlackBirdBox::KeyPressedEvent& e);

private:
    BlackBirdBox::Ref<BlackBirdNestLayer> black_bird_nest_layer_;
};
}
