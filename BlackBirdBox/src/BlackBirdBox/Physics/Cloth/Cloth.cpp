﻿#include "bbbpch.h"
#include "Cloth.h"

#include "BlackBirdBox/Core/Util.h"

namespace BlackBirdBox {

Cloth::Cloth(uint32_t num_cloth_dimension_size, Ref<Material> material_to_render_cloth)
    : num_cloth_dimension_size_(num_cloth_dimension_size)
    , Shape(material_to_render_cloth, nullptr, CreateRef<Transform>(glm::vec3(0, 0, 0)), ModelData(), "Cloth")
{
    PROFILE_FUNCTION();

    num_cloth_particles_ = num_cloth_dimension_size_ * num_cloth_dimension_size_;
    std::vector<glm::vec4> prev_cloth_particle_positons;
    prev_cloth_particle_positons.reserve(num_cloth_particles_);
    std::vector<glm::vec4> cloth_particle_positons;
    cloth_particle_positons.reserve(num_cloth_particles_);

    std::vector<glm::vec4> cloth_particle_fixed_pos;
    cloth_particle_fixed_pos.reserve(num_cloth_particles_);

    horizontal_vertical_distance_between_vertexes_ = 1.0f / static_cast<float>(num_cloth_dimension_size_);
    diagonal_distance_between_vertexes_ = horizontal_vertical_distance_between_vertexes_ * std::sqrt(2.0f);
    bend_distance_between_vertexes_ = 2.0f * horizontal_vertical_distance_between_vertexes_;
    for (uint32_t i = 0; i < num_cloth_particles_; ++i) {
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
        = ShaderStorageBuffer::Create(prev_cloth_particle_positons, prev_cloth_particle_positons.size() * sizeof(glm::vec4), false);
    auto positions_buffer = ShaderStorageBuffer::Create(cloth_particle_positons, cloth_particle_positons.size() * sizeof(glm::vec4), false);
    auto fixed_pos_buffer = ShaderStorageBuffer::Create(cloth_particle_fixed_pos, cloth_particle_fixed_pos.size() * sizeof(glm::vec4), true);
    cloth_storage_array_ = ShaderStorageArray::Create();
    cloth_storage_array_->AddShaderStorageBuffer(prev_positions_buffer);
    cloth_storage_array_->AddShaderStorageBuffer(positions_buffer);
    cloth_storage_array_->AddShaderStorageBuffer(fixed_pos_buffer);

    vertex_array_ = VertexArray::Create();
    auto vertex_buffer_cloth_positions = VertexBuffer::CreateExistingBuffer(positions_buffer->GetRendererID());
    BufferLayout layout_cloth_position = {
        { ShaderDataType::Float4, "a_Position" },
    };

    vertex_buffer_cloth_positions->SetLayout(layout_cloth_position);
    vertex_array_->AddVertexBuffer(vertex_buffer_cloth_positions);

    std::vector<uint32_t> cloth_indices;
    cloth_indices.reserve((num_cloth_dimension_size_ - 1) * (num_cloth_dimension_size_ - 1) * 6);
    for (uint32_t y = 0; y < num_cloth_dimension_size_ - 1; ++y) {
        for (uint32_t x = 0; x < num_cloth_dimension_size_ - 1; ++x) {
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

    Ref<IndexBuffer> index_buffer_cloth = IndexBuffer::Create(cloth_indices.data(), cloth_indices.size());
    vertex_array_->SetIndexBuffer(index_buffer_cloth);

    graph_colored_edges_ = Util::ClothSeperateEdges(num_cloth_dimension_size);

    for (size_t i = 0; i < graph_colored_edges_.size(); ++i) {
        batch_id_buffers_.push_back(ShaderStorageBuffer::Create(graph_colored_edges_[i], graph_colored_edges_[i].size() * sizeof(glm::vec4)));
    }

    batch_id_start_ind_ = cloth_storage_array_->AddShaderStorageBuffer(batch_id_buffers_[0]);

    auto mesh_triangles = Util::GetTrianglesOfSquaredMesh(num_cloth_dimension_size);
    auto mesh_triangles_shader_storage_buffer = ShaderStorageBuffer::Create(mesh_triangles, mesh_triangles.size() * sizeof(glm::uvec4));
    cloth_storage_array_->AddShaderStorageBuffer(mesh_triangles_shader_storage_buffer);

    auto cloth_normals_storage_buffer
        = ShaderStorageBuffer::Create(cloth_particle_positons, cloth_particle_positons.size() * sizeof(glm::vec4), false);
    cloth_storage_array_->AddShaderStorageBuffer(cloth_normals_storage_buffer);

    auto vertex_buffer_cloth_normals = VertexBuffer::CreateExistingBuffer(cloth_normals_storage_buffer->GetRendererID());
    BufferLayout layout_cloth_normal = {
        { ShaderDataType::Float4, "a_Normal" },
    };

    vertex_buffer_cloth_normals->SetLayout(layout_cloth_normal);
    vertex_array_->AddVertexBuffer(vertex_buffer_cloth_normals);
}

Ref<ShaderStorageArray> Cloth::GetClothStorageArray(int batch_id)
{
    PROFILE_FUNCTION();

    cloth_storage_array_->SetShaderStorageBuffer(batch_id_start_ind_, batch_id_buffers_[batch_id]);
    return cloth_storage_array_;
}

}
