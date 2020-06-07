#pragma once

#include "glm/glm.hpp"

namespace Core {
	class ComputeShaderSimulationConfiguration
	{
	public:
		ComputeShaderSimulationConfiguration();
		ComputeShaderSimulationConfiguration(glm::vec3 gravity, float delta_time, int itterations, float rest_lenght);

		const glm::vec3& GetGravity() const { return gravity_; }
		void SetGravity(const glm::vec3& gravity) { gravity_ = gravity; }

		float GetDeltaTime() const { return delta_time_; }
		void SetDeltaTime(float delta_time)  { delta_time_ = delta_time; }

		int GetItterations() const { return itterations_; }
		void SetItterations(int itterations) { itterations_ = itterations; }

		float GetRestLenght() const { return rest_lenght_; }
		void SetRestLenght(float rest_lenght) { rest_lenght_ = rest_lenght; }


	private:
		glm::vec3 gravity_;
		float delta_time_;
		int itterations_;
		float rest_lenght_;
	};
}