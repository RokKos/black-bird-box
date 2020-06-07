#include "bbbpch.h"
#include "CameraMenu.h"

#include <imgui.h>

namespace Core {

	CameraMenu::CameraMenu(const std::string& menu_name, Ref<PerspectiveCameraController> perspective_camera_controller) :
		BaseMenu(menu_name), perspective_camera_controller_(perspective_camera_controller)
	{

	}

	void CameraMenu::OnImGuiRender()
	{
		ImGui::Begin(menu_name_.c_str());

		float camera_movement_speed = perspective_camera_controller_->GetCameraMovementSpeed();
		ImGui::InputFloat("Camera movement speed", &camera_movement_speed);
		perspective_camera_controller_->SetCameraMovementSpeed(camera_movement_speed);

		float camera_rotation_speed = perspective_camera_controller_->GetCameraRotationSpeed();
		ImGui::InputFloat("Camera rotation speed", &camera_rotation_speed);
		perspective_camera_controller_->SetCameraRotationSpeed(camera_rotation_speed);

		ImGui::Text("Yaw: %f", perspective_camera_controller_->GetYaw());
		ImGui::Text("Pitch: %f", perspective_camera_controller_->GetPitch());
		auto camera_front = perspective_camera_controller_->GetCamerFront();
		ImGui::Text("Camera front x: %f y: %f z: %f", camera_front.x, camera_front.y, camera_front.z);

		ImGui::End();
	}

}