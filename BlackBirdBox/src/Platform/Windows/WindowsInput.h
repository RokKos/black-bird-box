#pragma once

#include "BlackBirdBox/Core/Input.h"

namespace Platform {

class WindowsInput : public BlackBirdBox::Input {
protected:
    virtual bool IsKeyPressedImpl(BlackBirdBox::KeyCode key) override;

    virtual bool IsMouseButtonPressedImpl(BlackBirdBox::MouseCode button) override;
    virtual std::pair<float, float> GetMousePositionImpl() override;
    virtual float GetMouseXImpl() override;
    virtual float GetMouseYImpl() override;
};

}
