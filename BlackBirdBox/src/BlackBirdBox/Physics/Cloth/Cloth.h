#pragma once

#include "BlackBirdBox/Components/Material.h"
#include "BlackBirdBox/Primitives/Shape.h"
#include "BlackBirdBox/Renderer/Buffers/ShaderStorageArray.h"
#include "BlackBirdBox/Renderer/Buffers/ShaderStorageBuffer.h"

namespace Core
{
    class Cloth : public Shape
    {
    public:
        Cloth(unsigned int num_cloth_dimension_size, Ref<Material> material_to_render_cloth);

        Ref<ShaderStorageArray> GetClothStorageArray(int batch_id);
        const std::vector<std::vector<glm::vec4>> &GetBatches() const { return graph_colored_edges_; };
        size_t GetNumberOfBatches() const { return batch_id_buffers_.size(); };
        float GetHorizontalVerticalDistanceBetweenVertexes() const
        {
            return horizontal_vertical_distance_between_vertexes_;
        };
        float GetDiagonalDistanceBetweenVertexes() const { return diagonal_distance_between_vertexes_; };

    private:
        Ref<ShaderStorageArray> cloth_storage_array_;
        std::vector<Ref<ShaderStorageBuffer>> batch_id_buffers_;
        std::vector<std::vector<glm::vec4>> graph_colored_edges_;
        size_t batch_id_start_ind_ = 0;
        unsigned int num_cloth_dimension_size_;
        unsigned int num_cloth_particles_;

        float horizontal_vertical_distance_between_vertexes_ = 0.0f;
        float diagonal_distance_between_vertexes_ = 0.0f;
    };
} // namespace Core
