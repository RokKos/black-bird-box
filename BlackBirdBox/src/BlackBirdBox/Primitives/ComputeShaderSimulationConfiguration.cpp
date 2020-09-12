#include "bbbpch.h"

#include "ComputeShaderSimulationConfiguration.h"

namespace Core {

ComputeShaderSimulationConfiguration::ComputeShaderSimulationConfiguration(glm::vec3 gravity, float delta_time, int iterations,
    float horizontal_vertical_distance_between_vertexes_, float diagonal_distance_between_vertexes)
    : gravity_(gravity)
    , delta_time_(delta_time)
    , iterations_(iterations)
    , horizontal_vertical_distance_between_vertexes_(horizontal_vertical_distance_between_vertexes_)
    , diagonal_distance_between_vertexes_(diagonal_distance_between_vertexes)
{
    PROFILE_FUNCTION();
}

ComputeShaderSimulationConfiguration::ComputeShaderSimulationConfiguration()
    : gravity_(glm::vec3(0.0f, -9.81f, 0.0f))
    , delta_time_(0.001f)
    , iterations_(30)
    , horizontal_vertical_distance_between_vertexes_(1.0f)
    , diagonal_distance_between_vertexes_(1.0f)
{
    PROFILE_FUNCTION();
}

void ComputeShaderSimulationConfiguration::SetGravity(const glm::vec3& gravity) { gravity_ = gravity; }

void ComputeShaderSimulationConfiguration::SetDeltaTime(float delta_time) { delta_time_ = delta_time; }

void ComputeShaderSimulationConfiguration::SetIterations(int iterations) { iterations_ = iterations; }

void ComputeShaderSimulationConfiguration::SetHorizontalVerticalDistanceBetweenVertexes(float horizontal_vertical_distance_between_vertexes)
{
    horizontal_vertical_distance_between_vertexes_ = horizontal_vertical_distance_between_vertexes;
}

void ComputeShaderSimulationConfiguration::SetDiagonalDistanceBetweenVertexes(float diagonal_distance_between_vertexes)
{
    diagonal_distance_between_vertexes_ = diagonal_distance_between_vertexes;
}
} // namespace Core
