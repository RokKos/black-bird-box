#include "bbbpch.h"
#include "ComputeShaderSimulationConfiguration.h"

namespace Core {

	ComputeShaderSimulationConfiguration::ComputeShaderSimulationConfiguration(glm::vec3 gravity, float delta_time, int itterations, float rest_lenght) :
		gravity_(gravity), delta_time_(delta_time), itterations_(itterations), rest_lenght_(rest_lenght)
	{

	}

	ComputeShaderSimulationConfiguration::ComputeShaderSimulationConfiguration():
		gravity_(glm::vec3(0.0f, -9.81f, 0.0f)), delta_time_(0.001f), itterations_(30), rest_lenght_(1.0 / 50.0f)
	{

	}

}