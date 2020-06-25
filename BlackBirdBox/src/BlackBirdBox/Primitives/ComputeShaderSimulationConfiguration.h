#pragma once

#include "glm/glm.hpp"

namespace Core {
	class ComputeShaderSimulationConfiguration
	{
	public:
		ComputeShaderSimulationConfiguration();
		ComputeShaderSimulationConfiguration(glm::vec3 gravity, float delta_time, int itterations);

		const glm::vec3& GetGravity() const { return gravity_; }
		void SetGravity(const glm::vec3& gravity) { gravity_ = gravity; }

		float GetDeltaTime() const { return delta_time_; }
		void SetDeltaTime(float delta_time)  { delta_time_ = delta_time; }

		int GetItterations() const { return itterations_; }
		void SetItterations(int itterations) { itterations_ = itterations; }


	private:
		glm::vec3 gravity_;
		float delta_time_;
		int itterations_;
	};
}