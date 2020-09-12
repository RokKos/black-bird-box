#include "bbbpch.h"
#include "ComputeShaderMenu.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace Core {

ComputeShaderMenu::ComputeShaderMenu(const std::string& menu_name, ComputeShaderSimulationConfiguration& compute_shader_simulation_configuration)
    : BaseMenu(menu_name)
    , compute_shader_simulation_configuration_(compute_shader_simulation_configuration)
{
    PROFILE_FUNCTION();
}

void ComputeShaderMenu::OnImGuiRender()
{
    PROFILE_FUNCTION();

    ImGui::Begin(menu_name_.c_str());

    glm::vec3 gravity = compute_shader_simulation_configuration_.GetGravity();
    ImGui::InputFloat3("Gravity", glm::value_ptr(gravity), 10);
    compute_shader_simulation_configuration_.SetGravity(gravity);

    float delta_time = compute_shader_simulation_configuration_.GetDeltaTime();
    ImGui::InputFloat("Delta Time", &delta_time, 0.0f, 0.0f, "%.10f");
    compute_shader_simulation_configuration_.SetDeltaTime(delta_time);

    int itterations = compute_shader_simulation_configuration_.GetIterations();
    ImGui::InputInt("Itterations", &itterations);
    compute_shader_simulation_configuration_.SetIterations(itterations);

    ImGui::End();
}

}
