#pragma once

#include "BaseMenu.h"
#include "BlackBirdBox/Controllers/Renderer/Camera/PerspectiveCameraController.h"

namespace Core {

	class CameraMenu : public BaseMenu
	{
	public:
		CameraMenu(const std::string& menu_name, Ref<PerspectiveCameraController> perspective_camera_controller);

		virtual void OnImGuiRender() override;

	private:
		Ref<PerspectiveCameraController> perspective_camera_controller_;
	};
}