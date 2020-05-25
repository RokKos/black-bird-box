#pragma once

#include "BlackBirdBox/Renderer/Cameras/PerspectiveCamera.h"
#include "BlackBirdBox/Core/TimeStep.h"

#include "BlackBirdBox/Events/ApplicationEvent.h"
#include "BlackBirdBox/Events/MouseEvent.h"

namespace Core {

	class PerspectiveCameraController
	{
	public:
		PerspectiveCameraController();
		PerspectiveCameraController(float aspect_ratio, float field_of_view, float near_clipping_plane, float far_clipping_plane);

		void OnUpdate(TimeStep ts);
		void OnEvent(Event& e);

		PerspectiveCamera& GetCamera() { return camera_; }
		const PerspectiveCamera& GetCamera() const { return camera_; }

		float GetZoomLevel() const { return zoom_level_; }
		void SetZoomLevel(float level) { zoom_level_ = level; }

		float GetCameraMovementSpeed() const { return camera_movement_speed_; }
		void SetCameraMovementSpeed(float camera_speed) { camera_movement_speed_ = camera_speed; }

		float GetCameraRotationSpeed() const { return camera_rotation_speed_; }
		void SetCameraRotationSpeed(float camera_speed) { camera_rotation_speed_ = camera_speed; }
		

		float GetYaw() const { return yaw_; }
		float GetPitch() const { return pitch_; }
		const glm::vec3& GetCamerFront() const { return camera_.GetRotation(); }

	private:
		bool OnMouseMove(MouseMovedEvent& e);
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
		void SetCameraRotation();


	private:
		PerspectiveCamera camera_;

		glm::vec3 camera_up_ = {0.0f, 1.0f, 0.0f};

		float zoom_level_ = 45.0f;
		float camera_movement_speed_ = 1.0f;
		float camera_rotation_speed_ = 0.1f;

		glm::vec2 prev_mouse_pos_;
		float yaw_ = 90.0f;
		float pitch_ = 0.0f;
	};

}