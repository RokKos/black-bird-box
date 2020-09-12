#pragma once

#include "glm/glm.hpp"

namespace Core {
class ComputeShaderSimulationConfiguration {
public:
    ComputeShaderSimulationConfiguration();
    ComputeShaderSimulationConfiguration(glm::vec3 gravity, float delta_time, int iterations,
        float horizontal_vertical_distance_between_vertexes = 0.0f, float diagonal_distance_between_vertexes = 0.0f);

    const glm::vec3& GetGravity() const { return gravity_; }
    void SetGravity(const glm::vec3& gravity);

    float GetDeltaTime() const { return delta_time_; }
    void SetDeltaTime(float delta_time);

    int GetIterations() const { return iterations_; }
    void SetIterations(int iterations);

    float GetHorizontalVerticalDistanceBetweenVertexes() const { return horizontal_vertical_distance_between_vertexes_; }
    void SetHorizontalVerticalDistanceBetweenVertexes(float horizontal_vertical_distance_between_vertexes);

    float GetDiagonalDistanceBetweenVertexes() const { return diagonal_distance_between_vertexes_; }
    void SetDiagonalDistanceBetweenVertexes(float diagonal_distance_between_vertexes);

private:
    glm::vec3 gravity_;
    float delta_time_;
    int iterations_;
    float horizontal_vertical_distance_between_vertexes_ = 0.0f;
    float diagonal_distance_between_vertexes_ = 0.0f;
};
} // namespace Core
