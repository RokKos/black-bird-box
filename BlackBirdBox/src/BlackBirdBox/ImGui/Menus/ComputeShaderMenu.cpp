#include "bbbpch.h"
#include "ComputeShaderMenu.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace BlackBirdBox {

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

    ImGui::Text("Verlet Integration Compute Shader Parameters");

    glm::vec3 gravity = compute_shader_simulation_configuration_.GetGravity();
    ImGui::InputFloat3("Gravity", glm::value_ptr(gravity), 10);
    compute_shader_simulation_configuration_.SetGravity(gravity);

    float delta_time = compute_shader_simulation_configuration_.GetDeltaTime();
    ImGui::InputFloat("Delta Time", &delta_time, 0.0f, 0.0f, "%.10f");
    compute_shader_simulation_configuration_.SetDeltaTime(delta_time);

    glm::vec3 external_force = compute_shader_simulation_configuration_.GetExternalForce();
    ImGui::InputFloat3("External Force", glm::value_ptr(external_force), 10);
    compute_shader_simulation_configuration_.SetExternalForce(external_force);

    glm::vec3 wind_resistance = compute_shader_simulation_configuration_.GetWindResistance();
    ImGui::InputFloat3("Wind Resistance", glm::value_ptr(wind_resistance), 10);
    compute_shader_simulation_configuration_.SetWindResistance(wind_resistance);

    ImGui::Text("Constraint Compute Shader Parameters");

    int iterations = compute_shader_simulation_configuration_.GetIterations();
    ImGui::InputInt("Iterations", &iterations);
    compute_shader_simulation_configuration_.SetIterations(iterations);

    float horizontal_vertical_distance = compute_shader_simulation_configuration_.GetHorizontalVerticalDistanceBetweenVertexes();
    ImGui::InputFloat("Horizontal Vertical Distance Between Vertexes", &horizontal_vertical_distance, 0.0f, 0.0f, "%.10f");
    compute_shader_simulation_configuration_.SetHorizontalVerticalDistanceBetweenVertexes(horizontal_vertical_distance);

    float diagonal_vertical_distance = compute_shader_simulation_configuration_.GetDiagonalDistanceBetweenVertexes();
    ImGui::InputFloat("Diagonal Distance Between Vertexes", &diagonal_vertical_distance, 0.0f, 0.0f, "%.10f");
    compute_shader_simulation_configuration_.SetDiagonalDistanceBetweenVertexes(diagonal_vertical_distance);

    float bend_distance = compute_shader_simulation_configuration_.GetBendDistanceBetweenVertexes();
    ImGui::InputFloat("Bend Between Vertexes", &bend_distance, 0.0f, 0.0f, "%.10f");
    compute_shader_simulation_configuration_.SetBendDistanceBetweenVertexes(bend_distance);

    float structural_stiffness = compute_shader_simulation_configuration_.GetStructuralStiffness();
    ImGui::InputFloat("Structural Stiffness", &structural_stiffness, 0.0f, 0.0f, "%.10f");
    compute_shader_simulation_configuration_.SetStructuralStiffness(structural_stiffness);

    float shear_stiffness = compute_shader_simulation_configuration_.GetShearStiffness();
    ImGui::InputFloat("Shear Stiffness", &shear_stiffness, 0.0f, 0.0f, "%.10f");
    compute_shader_simulation_configuration_.SetShearStiffness(shear_stiffness);

    float flexion_stiffness = compute_shader_simulation_configuration_.GetFlexionStiffness();
    ImGui::InputFloat("Flexion Stiffness", &flexion_stiffness, 0.0f, 0.0f, "%.10f");
    compute_shader_simulation_configuration_.SetFlexionStiffness(flexion_stiffness);

    ImGui::End();
}

}
