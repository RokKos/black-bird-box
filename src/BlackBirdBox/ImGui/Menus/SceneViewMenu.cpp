#include "bbbpch.h"
#include "SceneViewMenu.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace Core {


	SceneViewMenu::SceneViewMenu(const std::string& menu_name, const std::vector<Ref<Shape>>& shapes, const std::vector<Ref<LightSource>>& lights) :
		BaseMenu(menu_name), shapes_(shapes), lights_(lights)
	{

	}

	void SceneViewMenu::OnImGuiRender()
	{
		ImGui::Begin(menu_name_.c_str());

		for (auto shape : shapes_)
		{
			if (ImGui::TreeNode(shape->GetName().c_str())) {
				ImGui::Checkbox("Enabled", shape->GetObjectEnabledImGui());
				if (ImGui::TreeNode("Transform")) {
					// TODO(Rok Kos): Refac this
					auto shape_transform = shape->GetTransform();

					glm::vec3 t_pos = shape_transform->GetPosition();
					float pos[3] = { t_pos.x, t_pos.y, t_pos.z };
					ImGui::Text("Pos:"); ImGui::InputFloat3("a", &pos[0]);
					t_pos.x = pos[0];
					t_pos.y = pos[1];
					t_pos.z = pos[2];
					shape_transform->SetPosition(t_pos);


					glm::vec3 t_rot = shape_transform->GetRotation();
					float rot[3] = { t_rot.x, t_rot.y, t_rot.z };
					ImGui::Text("Rot:"); ImGui::InputFloat3("b", &rot[0]);
					t_rot.x = rot[0];
					t_rot.y = rot[1];
					t_rot.z = rot[2];
					shape_transform->SetRotation(t_rot);



					glm::vec3 t_scale = shape_transform->GetScale();
					float scl[3] = { t_scale.x, t_scale.y, t_scale.z };
					ImGui::Text("Scale:"); ImGui::InputFloat3("c", &scl[0]);
					t_scale.x = scl[0];
					t_scale.y = scl[1];
					t_scale.z = scl[2];
					shape_transform->SetScale(t_scale);

					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Material")) {
					auto shape_material = shape->GetMaterial();
					ImGui::Text(shape_material->GetName().c_str());
					auto lighting_data = shape_material->GetPhongLightingParameters();

					ImGui::ColorEdit3("Diffuse Color:", glm::value_ptr(lighting_data.diffuse_color_));
					ImGui::ColorEdit3("Specular Color:", glm::value_ptr(lighting_data.specular_color_));
					ImGui::ColorEdit3("Ambient Color:", glm::value_ptr(lighting_data.ambient_color_));
					ImGui::InputFloat3("Ambient Intensity:", glm::value_ptr(lighting_data.ambient_intensity_));

					ImGui::SliderFloat("Specular scattering:", &lighting_data.specular_scattering_, 0.0f, 256.0f);

					shape_material->SetPhongLightingParameters(lighting_data);

					ImGui::TreePop();
				}


				ImGui::TreePop();
			}
		}

		for (auto light_source : lights_)
		{
			if (ImGui::TreeNode(light_source->GetName().c_str())) {
				if (ImGui::TreeNode("Color Properties")) {
					glm::vec3 t_color = light_source->GetColor();
					ImGui::ColorEdit3("Color:", glm::value_ptr(t_color));
					light_source->SetColor(t_color);

					ImGui::TreePop();
				}


				if (ImGui::TreeNode("Transform")) {
					// TODO(Rok Kos): Refac this


					glm::vec3 t_pos = light_source->GetPosition();
					float pos[3] = { t_pos.x, t_pos.y, t_pos.z };
					ImGui::Text("Pos:"); ImGui::InputFloat3("a", &pos[0]);
					t_pos.x = pos[0];
					t_pos.y = pos[1];
					t_pos.z = pos[2];
					light_source->SetPosition(t_pos);


					glm::vec3 t_rot = light_source->GetDirection();
					float rot[3] = { t_rot.x, t_rot.y, t_rot.z };
					ImGui::Text("Rot:"); ImGui::InputFloat3("b", &rot[0]);
					t_rot.x = rot[0];
					t_rot.y = rot[1];
					t_rot.z = rot[2];
					light_source->SetDirection(t_rot);



					glm::vec3 t_scale = light_source->GetIntensity();
					float scl[3] = { t_scale.x, t_scale.y, t_scale.z };
					ImGui::Text("Scale:"); ImGui::InputFloat3("c", &scl[0]);
					t_scale.x = scl[0];
					t_scale.y = scl[1];
					t_scale.z = scl[2];
					light_source->SetIntensity(t_scale);

					ImGui::TreePop();
				}
				ImGui::TreePop();
			}
		}
		ImGui::End();
	}

}