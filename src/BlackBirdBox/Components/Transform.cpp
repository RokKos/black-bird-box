#include "bbbpch.h"
#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Core {


	Transform::Transform(glm::vec3 position) :
		position_(position), 
		rotation_(glm::vec3(1.0f)), 
		scale_(glm::vec3(1.0f))
	{
		PROFILE_FUNCTION();

		RecalculateTransformMatrix();
	}

	Transform::Transform(glm::vec3 position, glm::vec3 rotation) :
		position_(position),
		rotation_(rotation),
		scale_(glm::vec3(1.0f))
	{
		PROFILE_FUNCTION();

		RecalculateTransformMatrix();
	}

	Transform::Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) :
		position_(position),
		rotation_(rotation),
		scale_(scale)
	{
		PROFILE_FUNCTION();

		RecalculateTransformMatrix();
	}

	void Transform::RecalculateTransformMatrix()
	{
		PROFILE_FUNCTION();

		glm::mat4 transformation_matrix = glm::mat4(1.0f);
		transformation_matrix = glm::translate(transformation_matrix, position_);
		
		transformation_matrix = glm::rotate(transformation_matrix, glm::radians(rotation_.z), glm::vec3(0.0f, 0.0f, 1.0f));
		transformation_matrix = glm::rotate(transformation_matrix, glm::radians(rotation_.y), glm::vec3(0.0f, 1.0f, 0.0f));
		transformation_matrix = glm::rotate(transformation_matrix, glm::radians(rotation_.x), glm::vec3(1.0f, 0.0f, 0.0f));
		
		transformation_matrix = glm::scale(transformation_matrix, scale_);

		matrix_ = transformation_matrix;
	}

}
