#include "bbbpch.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace Core {
	
	void Camera::RecalculateViewMatrix()
	{
		PROFILE_FUNCTION();

		glm::vec3 camera_up = glm::vec3(0.f, 1.f, 0.f);
		view_matrix_ = glm::lookAt(position_, position_ + rotation_, camera_up);
		view_projection_matrix_ = projection_matrix_ * view_matrix_;
	}

}