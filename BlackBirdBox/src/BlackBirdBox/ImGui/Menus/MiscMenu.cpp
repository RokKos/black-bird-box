#include "bbbpch.h"
#include "MiscMenu.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace BlackBirdBox {

MiscMenu::MiscMenu(const std::string& menu_name, TimeStep& prev_time_step, glm::vec4& bg_color, int& polygon_mode)
    : BaseMenu(menu_name)
    , prev_time_step_(prev_time_step)
    , bg_color_(bg_color)
    , polygon_mode_(polygon_mode)
{
    PROFILE_FUNCTION();
}

void MiscMenu::OnImGuiRender()
{
    PROFILE_FUNCTION();

    ImGui::Begin(menu_name_.c_str());

    ImGui::ColorEdit4("BG Color", glm::value_ptr(bg_color_));
    ImGui::Combo("Polygon Mode", &polygon_mode_, polygon_mode_names_, IM_ARRAYSIZE(polygon_mode_names_));
    ImGui::Text("Delta time: %f", prev_time_step_.GetSeconds());

    ImGui::End();
}

}
