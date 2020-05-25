#pragma once

#include "glm/glm.hpp"

namespace Core {
	class Transform
	{
	public:
		Transform() = default;

		Transform(glm::vec3 position);
		Transform(glm::vec3 position, glm::vec3 rotation);
		Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

		const glm::mat4 GetTransformMatrix() const { return matrix_; }

		const glm::vec3& GetPosition() const { return position_; }
		const glm::vec3& GetRotation() const { return rotation_; }
		const glm::vec3& GetScale() const { return scale_; }

		void SetPosition(glm::vec3 position) { position_ = position; RecalculateTransformMatrix(); }
		void SetRotation(glm::vec3 rotation) { rotation_ = rotation; RecalculateTransformMatrix(); }
		void SetScale(glm::vec3 scale) { scale_ = scale; RecalculateTransformMatrix(); }
		

	private:
		void RecalculateTransformMatrix();

	private:
		glm::mat4 matrix_ = glm::mat4(1.0f);

		glm::vec3 position_ = glm::vec3(1.0f);
		glm::vec3 rotation_ = glm::vec3(1.0f);
		glm::vec3 scale_ = glm::vec3(1.0f);

		
	};

}