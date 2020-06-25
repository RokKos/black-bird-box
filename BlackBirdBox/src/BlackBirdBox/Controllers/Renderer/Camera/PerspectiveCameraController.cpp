#include "bbbpch.h"
#include "PerspectiveCameraController.h"

#include "BlackBirdBox/Core/Input.h"

namespace Core {
	

	PerspectiveCameraController::PerspectiveCameraController() :
		camera_(16.0f / 9.0f, 45.0f, 0.01f, 1000.0f), prev_mouse_pos_(600.0f, 400.0f)
	{
		PROFILE_FUNCTION();

		SetCameraRotation();
	}

	PerspectiveCameraController::PerspectiveCameraController(float aspect_ratio, float field_of_view, float near_clipping_plane, float far_clipping_plane) :
		camera_(aspect_ratio, field_of_view, near_clipping_plane, far_clipping_plane), prev_mouse_pos_(600.0f, 400.0f)
	{
		PROFILE_FUNCTION();

		SetCameraRotation();
	}

	void PerspectiveCameraController::OnUpdate(TimeStep ts)
	{
		PROFILE_FUNCTION();

		glm::vec3 camera_pos = camera_.GetPosition();

		if (Input::IsKeyPressed(KeyCode::A))
		{
			camera_pos -= glm::normalize(glm::cross(camera_.GetRotation(), camera_up_)) * camera_movement_speed_ * ts.GetSeconds();
		}
		else if (Input::IsKeyPressed(KeyCode::D)) {
			camera_pos += glm::normalize(glm::cross(camera_.GetRotation(), camera_up_)) * camera_movement_speed_ * ts.GetSeconds();
		}

		if (Input::IsKeyPressed(KeyCode::W))
		{
			camera_pos += camera_movement_speed_ * camera_.GetRotation() * ts.GetSeconds();
		}
		else if (Input::IsKeyPressed(KeyCode::S)) {
			camera_pos -= camera_movement_speed_ * camera_.GetRotation() * ts.GetSeconds();
		}

		camera_.SetPosition(camera_pos);
	}

	void PerspectiveCameraController::OnEvent(Event& e)
	{
		PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(PerspectiveCameraController::OnMouseMove));
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(PerspectiveCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(PerspectiveCameraController::OnWindowResized));
	}

	bool PerspectiveCameraController::OnMouseMove(MouseMovedEvent& e)
	{
		PROFILE_FUNCTION();

		glm::vec2 mouse_pos = glm::vec2(e.GetX(), e.GetY());

		if (Input::IsMouseButtonPressed(MouseCode::ButtonRight) || Input::IsMouseButtonPressed(MouseCode::ButtonMiddle)) {
			glm::vec2 offset = mouse_pos - prev_mouse_pos_;
			offset *= camera_rotation_speed_;

			yaw_ += offset.x;
			pitch_ = glm::clamp(pitch_ - offset.y, -89.9f, 89.9f);

			SetCameraRotation();
		}

		prev_mouse_pos_ = mouse_pos;

		return false;
	}

	bool PerspectiveCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		PROFILE_FUNCTION();

		zoom_level_ -= e.GetYOffset() * 0.25f;
		zoom_level_ = std::max(std::min(zoom_level_, 45.0f), 1.0f);
		camera_.SetFieldOfView(zoom_level_);
		return false;
	}

	bool PerspectiveCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		PROFILE_FUNCTION();

		auto aspect_ratio = (float)e.GetWidth() / (float)e.GetHeight();
		camera_.SetAspectRatio(aspect_ratio);
		return false;
	}

	void PerspectiveCameraController::SetCameraRotation()
	{
		PROFILE_FUNCTION();

		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
		direction.y = sin(glm::radians(pitch_));
		direction.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
		camera_.SetRotation(glm::normalize(direction));
	}

}