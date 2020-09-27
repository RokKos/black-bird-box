#pragma once

#include "BaseMenu.h"
#include "BlackBirdBox/Core/TimeStep.h"

#include "glm/glm.hpp"

namespace BlackBirdBox {

class MiscMenu : public BaseMenu {
public:
    MiscMenu(const std::string& menu_name, TimeStep& prev_time_step, glm::vec4& bg_color_, int& polygon_mode_);

    virtual void OnImGuiRender() override;

private:
    TimeStep& prev_time_step_;
    glm::vec4& bg_color_;
    int& polygon_mode_;
    const char* polygon_mode_names_[3]{ "Point", "Line", "Fill" };
};
}
