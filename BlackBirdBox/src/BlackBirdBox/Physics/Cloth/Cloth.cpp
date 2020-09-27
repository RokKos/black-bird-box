#include "bbbpch.h"
#include "Cloth.h"

#include "BlackBirdBox/Core/Util.h"

namespace BlackBirdBox {

Cloth::Cloth(unsigned int num_cloth_dimension_size, Ref<Material> material_to_render_cloth)
    : num_cloth_dimension_size_(num_cloth_dimension_size)
    , Shape(
          material_to_render_cloth, nullptr, BlackBirdBox::CreateRef<BlackBirdBox::Transform>(glm::vec3(0, 0, 0)), BlackBirdBox::ModelData(), "Cloth")
{
    PROFILE_FUNCTION();

    num_cloth_particles_ = num_cloth_dimension_size_ * num_cloth_dimension_size_;
    std::vector<glm::vec4> prev_cloth_particle_positons;
    prev_cloth_particle_positons.reserve(num_cloth_particles_);
    std::vector<glm::vec4> cloth_particle_positons;
    cloth_particle_positons.reserve(num_cloth_particles_);

    std::vector<glm::vec4> cloth_particle_fixed_pos;
    cloth_particle_fixed_pos.reserve(num_cloth_particles_);

    horizontal_vertical_distance_between_vertexes_ = 1.0f / static_cast<float>(num_cloth_dimension_size_ - 1);
    diagonal_distance_between_vertexes_ = horizontal_vertical_distance_between_vertexes_ * std::sqrt(2.0f);
    bend_distance_between_vertexes_ = 2.0f * horizontal_vertical_distance_between_vertexes_;
    for (unsigned int i = 0; i < num_cloth_particles_; ++i) {
        int x = i % num_cloth_dimension_size_;
        int y = i / num_cloth_dimension_size_;
        glm::vec4 starting_position = glm::vec4((float)(x) / (float)num_cloth_dimension_size_, (float)(y) / (float)num_cloth_dimension_size_, 0, 0);
        prev_cloth_particle_positons.push_back(starting_position);
        cloth_particle_positons.push_back(starting_position);

        if (i == num_cloth_particles_ - 1 || i == num_cloth_particles_ - num_cloth_dimension_size_) {
            cloth_particle_fixed_pos.emplace_back(glm::vec4(starting_position.x, starting_position.y, starting_position.z, 1.0f));
        } else {
            cloth_particle_fixed_pos.emplace_back(glm::vec4(starting_position.x, starting_position.y, starting_position.z, 0.0f));
        }
    }

    auto prev_positions_buffer
        = BlackBirdBox::ShaderStorageBuffer::Create(prev_cloth_particle_positons, prev_cloth_particle_positons.size() * sizeof(glm::vec4), false);
    auto positions_buffer
        = BlackBirdBox::ShaderStorageBuffer::Create(cloth_particle_positons, cloth_particle_positons.size() * sizeof(glm::vec4), false);
    auto fixed_pos_buffer
        = BlackBirdBox::ShaderStorageBuffer::Create(cloth_particle_fixed_pos, cloth_particle_fixed_pos.size() * sizeof(glm::vec4), true);
    cloth_storage_array_ = BlackBirdBox::ShaderStorageArray::Create();
    cloth_storage_array_->AddShaderStorageBuffer(prev_positions_buffer);
    cloth_storage_array_->AddShaderStorageBuffer(positions_buffer);
    cloth_storage_array_->AddShaderStorageBuffer(fixed_pos_buffer);

    vertex_array_ = BlackBirdBox::VertexArray::Create();
    auto vertex_buffer_cloth = BlackBirdBox::VertexBuffer::CreateExistingBuffer(positions_buffer->GetRendererID());
    BlackBirdBox::BufferLayout layout_cloth = {
        { BlackBirdBox::ShaderDataType::Float4, "a_Position" },
    };

    vertex_buffer_cloth->SetLayout(layout_cloth);
    vertex_array_->AddVertexBuffer(vertex_buffer_cloth);

    std::vector<uint32_t> cloth_indices;
    cloth_indices.reserve((num_cloth_dimension_size_ - 1) * (num_cloth_dimension_size_ - 1) * 6);
    for (unsigned int y = 0; y < num_cloth_dimension_size_ - 1; ++y) {
        for (unsigned int x = 0; x < num_cloth_dimension_size_ - 1; ++x) {
            // Left Triangle
            cloth_indices.push_back(x + y * num_cloth_dimension_size_);
            cloth_indices.push_back((x + 1) + y * num_cloth_dimension_size_);
            cloth_indices.push_back(x + (y + 1) * num_cloth_dimension_size_);

            // Right Triangle
            cloth_indices.push_back((x + 1) + y * num_cloth_dimension_size_);
            cloth_indices.push_back((x + 1) + (y + 1) * num_cloth_dimension_size_);
            cloth_indices.push_back(x + (y + 1) * num_cloth_dimension_size_);
        }
    }

    BlackBirdBox::Ref<BlackBirdBox::IndexBuffer> index_buffer_cloth = BlackBirdBox::IndexBuffer::Create(cloth_indices.data(), cloth_indices.size());
    vertex_array_->SetIndexBuffer(index_buffer_cloth);

    graph_colored_edges_ = Util::ClothSeperateEdges(num_cloth_dimension_size);

    for (size_t i = 0; i < graph_colored_edges_.size(); ++i) {
        batch_id_buffers_.push_back(
            BlackBirdBox::ShaderStorageBuffer::Create(graph_colored_edges_[i], graph_colored_edges_[i].size() * sizeof(glm::vec4), false));
    }

    batch_id_start_ind_ = cloth_storage_array_->AddShaderStorageBuffer(batch_id_buffers_[0]);
}

BlackBirdBox::Ref<BlackBirdBox::ShaderStorageArray> Cloth::GetClothStorageArray(int batch_id)
{
    PROFILE_FUNCTION();

    cloth_storage_array_->SetShaderStorageBuffer(batch_id_start_ind_, batch_id_buffers_[batch_id]);
    return cloth_storage_array_;
}

}
