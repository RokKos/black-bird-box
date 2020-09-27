#include "bbbpch.h"
#include "SceneViewMenu.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace BlackBirdBox {

SceneViewMenu::SceneViewMenu(const std::string& menu_name, const std::vector<Ref<Shape>>& shapes, const std::vector<Ref<LightSource>>& lights)
    : BaseMenu(menu_name)
    , shapes_(shapes)
    , lights_(lights)
{
    PROFILE_FUNCTION();
}

void SceneViewMenu::OnImGuiRender()
{
    PROFILE_FUNCTION();

    ImGui::Begin(menu_name_.c_str());

    for (auto shape : shapes_) {
        if (ImGui::TreeNode(shape->GetName().c_str())) {
            ImGui::Checkbox("Enabled", shape->GetObjectEnabledImGui());
            if (ImGui::TreeNode("Transform")) {
                // TODO(Rok Kos): Refac this
                auto shape_transform = shape->GetTransform();

                glm::vec3 t_pos = shape_transform->GetPosition();
                float pos[3] = { t_pos.x, t_pos.y, t_pos.z };
                ImGui::Text("Pos:");
                ImGui::InputFloat3("a", &pos[0]);
                t_pos.x = pos[0];
                t_pos.y = pos[1];
                t_pos.z = pos[2];
                shape_transform->SetPosition(t_pos);

                glm::vec3 t_rot = shape_transform->GetRotation();
                float rot[3] = { t_rot.x, t_rot.y, t_rot.z };
                ImGui::Text("Rot:");
                ImGui::InputFloat3("b", &rot[0]);
                t_rot.x = rot[0];
                t_rot.y = rot[1];
                t_rot.z = rot[2];
                shape_transform->SetRotation(t_rot);

                glm::vec3 t_scale = shape_transform->GetScale();
                float scl[3] = { t_scale.x, t_scale.y, t_scale.z };
                ImGui::Text("Scale:");
                ImGui::InputFloat3("c", &scl[0]);
                t_scale.x = scl[0];
                t_scale.y = scl[1];
                t_scale.z = scl[2];
                shape_transform->SetScale(t_scale);

                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Material")) {
                auto shape_material = shape->GetMaterial();
                ImGui::Text(shape_material->GetName().c_str());

                for (Uniform& uniform : shape_material->GetUniforms()) {
                    switch (uniform.type_) {
                    case UniformType::Int: {
                        ImGui::InputInt(uniform.name_.c_str(), &uniform.integer_value);
                        break;
                    }

                    case UniformType::Float: {
                        ImGui::InputFloat(uniform.name_.c_str(), &uniform.float_value);
                        break;
                    }

                    case UniformType::Vec2: {
                        ImGui::InputFloat2(uniform.name_.c_str(), glm::value_ptr(uniform.vec2_value));
                        break;
                    }

                    case UniformType::Vec3: {
                        if (uniform.name_.find("Color") != std::string::npos || uniform.name_.find("color") != std::string::npos) {
                            ImGui::ColorEdit3(uniform.name_.c_str(), glm::value_ptr(uniform.vec3_value));
                        } else {
                            ImGui::InputFloat3(uniform.name_.c_str(), glm::value_ptr(uniform.vec3_value));
                        }

                        break;
                    }

                    case UniformType::Vec4: {
                        if (uniform.name_.find("Color") != std::string::npos || uniform.name_.find("color") != std::string::npos) {
                            ImGui::ColorEdit4(uniform.name_.c_str(), glm::value_ptr(uniform.vec4_value));
                        } else {
                            ImGui::InputFloat4(uniform.name_.c_str(), glm::value_ptr(uniform.vec4_value));
                        }

                        break;
                    }
                    case UniformType::Mat4:
                    case UniformType::Sampler2D:
                    case UniformType::SamplerCube: {
                        // TODO(Rok Kos): Find A way
                        break;
                    }

                    default:
                        CORE_ASSERT(false, "Unsuported type on Uniform: %s", uniform.name_.c_str());
                    }
                }

                ImGui::TreePop();
            }

            ImGui::TreePop();
        }
    }

    for (auto light_source : lights_) {
        if (ImGui::TreeNode(light_source->GetName().c_str())) {
            ImGui::Checkbox("Enabled", light_source->GetObjectEnabledImGui());
            if (ImGui::TreeNode("Color Properties")) {
                glm::vec3 t_color = light_source->GetColor();
                ImGui::ColorEdit3("Color:", glm::value_ptr(t_color));
                light_source->SetColor(t_color);

                glm::vec3 t_intensity = light_source->GetIntensity();
                ImGui::Text("Intensity:");
                ImGui::InputFloat3("c", &t_intensity[0]);
                light_source->SetColor(t_intensity);

                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Transform")) {
                // TODO(Rok Kos): Refac this

                glm::vec3 t_pos = light_source->GetPosition();
                float pos[3] = { t_pos.x, t_pos.y, t_pos.z };
                ImGui::Text("Pos:");
                ImGui::InputFloat3("a", &pos[0]);
                t_pos.x = pos[0];
                t_pos.y = pos[1];
                t_pos.z = pos[2];
                light_source->SetPosition(t_pos);
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
    }
    ImGui::End();
}

}
