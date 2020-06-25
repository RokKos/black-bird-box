#pragma once

#include "glm/glm.hpp"

namespace Core {

	class Point
	{
	public:
		Point() = default;

		Point(float size, glm::vec3 position, glm::vec3 color);

		float GetSize() const { return size_; }
		const glm::vec3& GetPosition() const { return position_; }
		const glm::vec3& GetColor() const { return color_; }

	private:
		float size_ = 10.0f;
		glm::vec3 position_ = glm::vec3(1.0f);
		glm::vec3 color_ = glm::vec3(1.0f);
	};
}