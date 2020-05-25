#pragma once

#include <glm/glm.hpp>

namespace Core {
	class Camera
	{
	public:
		Camera() : 
			projection_matrix_(1.0f), view_matrix_(1.0f), view_projection_matrix_(1.0f) 
		{
		}

		Camera(glm::mat4 projection_matrix, glm::mat4 view_matrix) :
			projection_matrix_(projection_matrix), view_matrix_(view_matrix)
		{
			view_projection_matrix_ = projection_matrix_ * view_matrix_;
		}
		
		~Camera() = default;

		const glm::vec3& GetPosition() const { return position_; }
		void SetPosition(const glm::vec3& position) { position_ = position; RecalculateViewMatrix(); }

		const glm::vec3& GetRotation() const { return rotation_; }
		void SetRotation(const glm::vec3& rotation) { rotation_ = rotation; RecalculateViewMatrix(); }

		const glm::mat4& GetProjectionMatrix() const { return projection_matrix_; }
		const glm::mat4& GetViewMatrix() const { return view_matrix_; }
		const glm::mat4& GetViewProjectionMatrix() const { return view_projection_matrix_; }

	protected:
		void RecalculateViewMatrix();

	protected:
		glm::mat4 projection_matrix_;
		glm::mat4 view_matrix_;
		glm::mat4 view_projection_matrix_;

		glm::vec3 position_ = { 0.1f, 0.1f, -1.0f };
		glm::vec3 rotation_ = { 0.0f, 0.0f, 0.5f };
	};
}