#pragma once

#include "glm/glm.hpp"

namespace BlackBirdBox {
class ComputeShaderSimulationConfiguration {
public:
    ComputeShaderSimulationConfiguration();
    ComputeShaderSimulationConfiguration(glm::vec3 gravity, float delta_time, glm::vec3 external_force, glm::vec3 wind_resistance, int iterations,
        float structural_stiffness, float shear_stiffness, float flexion_stiffness, float horizontal_vertical_distance_between_vertexes = 0.0f,
        float diagonal_distance_between_vertexes = 0.0f, float bend_distance_between_vertexes = 0.0f);

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

    float GetBendDistanceBetweenVertexes() const { return bend_distance_between_vertexes_; }
    void SetBendDistanceBetweenVertexes(float bend_distance_between_vertexes);

    float GetStructuralStiffness() const { return structural_stiffness_; }
    void SetStructuralStiffness(float structural_stiffness);

    float GetShearStiffness() const { return shear_stiffness_; }
    void SetShearStiffness(float shear_stiffness);

    float GetFlexionStiffness() const { return flexion_stiffness_; }
    void SetFlexionStiffness(float flexion_stiffness);

    const glm::vec3& GetExternalForce() const { return external_force_; }
    void SetExternalForces(glm::vec3 external_force);

    const glm::vec3& GetWindResistance() const { return wind_resistance_; }
    void SetWindResistance(glm::vec3 wind_resistance);

private:
    glm::vec3 gravity_;
    float delta_time_;
    int iterations_;

    float horizontal_vertical_distance_between_vertexes_ = 0.0f;
    float diagonal_distance_between_vertexes_ = 0.0f;
    float bend_distance_between_vertexes_ = 0.0f;

    float structural_stiffness_ = 1.0f;
    float shear_stiffness_ = 1.0f;
    float flexion_stiffness_ = 1.0f;

    glm::vec3 external_force_;
    glm::vec3 wind_resistance_;
};
} // namespace BlackBirdBox
