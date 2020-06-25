#include "bbbpch.h"
#include "Cloth.h"

#include "BlackBirdBox/Core/Util.h"

namespace Core {

	Cloth::Cloth(unsigned int num_cloth_dimension_size, Ref<Material> material_to_render_cloth) :
		num_cloth_dimension_size_(num_cloth_dimension_size), Shape(material_to_render_cloth, nullptr, Core::CreateRef<Core::Transform>(glm::vec3(0, 0, 0)), Core::ModelData(), "Cloth")
	{
		PROFILE_FUNCTION();

		num_cloth_particles_ = num_cloth_dimension_size_ * num_cloth_dimension_size_;
		std::vector<glm::vec4> prev_cloth_particle_positons;
		prev_cloth_particle_positons.reserve(num_cloth_particles_);
		std::vector<glm::vec4> cloth_particle_positons;
		cloth_particle_positons.reserve(num_cloth_particles_);

		std::vector<glm::mat4> cloth_particle_constraints;
		cloth_particle_constraints.reserve(num_cloth_particles_);
		std::vector<glm::vec4> cloth_particle_fixed_pos;
		cloth_particle_fixed_pos.reserve(num_cloth_particles_);

		float horizontal_distance_between_vertexes = 1.0f / static_cast<float>(num_cloth_dimension_size_);
		float diagonal_distance_between_vertexes = horizontal_distance_between_vertexes * std::sqrt(2.0f);
		for (unsigned int i = 0; i < num_cloth_particles_; ++i) {
			int x = i % num_cloth_dimension_size_;
			int y = i / num_cloth_dimension_size_;
			glm::vec4 starting_position = glm::vec4((float)(x) / (float)num_cloth_dimension_size_, (float)(y) / (float)num_cloth_dimension_size_, 0, 0);
			prev_cloth_particle_positons.push_back(starting_position);
			cloth_particle_positons.push_back(starting_position);

			glm::mat4 contraint_indexs = glm::mat4(-1.0);
			for (int dx = -1; dx <= 1; ++dx) {
				for (int dy = -1; dy <= 1; ++dy) {
					int new_x = x + dx;
					int new_y = y + dy;

					if (new_x < 0 || new_y < 0 ||
						new_x >= num_cloth_dimension_size_ || new_y >= num_cloth_dimension_size_ ||
						(new_x == x && new_y == y)) {
						contraint_indexs[dx + 1][dy + 1] = -1.0f;
						continue;
					}

					contraint_indexs[dx + 1][dy + 1] = new_x + new_y * num_cloth_dimension_size_;
				}
			}

			cloth_particle_constraints.push_back(contraint_indexs);


			if (i == num_cloth_particles_ - 1 || i == num_cloth_particles_ - num_cloth_dimension_size_) {
				cloth_particle_fixed_pos.push_back(glm::vec4(1.0, horizontal_distance_between_vertexes, diagonal_distance_between_vertexes, 0.0f));
			}
			else {
				cloth_particle_fixed_pos.push_back(glm::vec4(0.0, horizontal_distance_between_vertexes, diagonal_distance_between_vertexes, 0.0f));
			}

		}

		auto prev_positions_buffer = Core::ShaderStorageBuffer::Create(prev_cloth_particle_positons, prev_cloth_particle_positons.size() * sizeof(glm::vec4));
		auto positions_buffer = Core::ShaderStorageBuffer::Create(cloth_particle_positons, cloth_particle_positons.size() * sizeof(glm::vec4));
		auto constrains_buffer = Core::ShaderStorageBuffer::Create(cloth_particle_constraints, cloth_particle_constraints.size() * sizeof(glm::mat4));
		auto fixed_pos_buffer = Core::ShaderStorageBuffer::Create(cloth_particle_fixed_pos, cloth_particle_fixed_pos.size() * sizeof(glm::vec4));
		cloth_storage_array_ = Core::ShaderStorageArray::Create();
		cloth_storage_array_->AddShaderStorageBuffer(prev_positions_buffer);
		cloth_storage_array_->AddShaderStorageBuffer(positions_buffer);
		cloth_storage_array_->AddShaderStorageBuffer(constrains_buffer);
		cloth_storage_array_->AddShaderStorageBuffer(fixed_pos_buffer);


		vertex_array_ = Core::VertexArray::Create();
		auto vertex_buffer_cloth = Core::VertexBuffer::CreateExistingBuffer(positions_buffer->GetRendererID());
		Core::BufferLayout layout_cloth = {
		{ Core::ShaderDataType::Float4, "a_Position" },
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

		Core::Ref<Core::IndexBuffer> index_buffer_cloth = Core::IndexBuffer::Create(cloth_indices.data(), cloth_indices.size());
		vertex_array_->SetIndexBuffer(index_buffer_cloth);

		std::vector<unsigned int> graph_coloring = Util::GreedyGraphColoring(num_cloth_particles_, cloth_particle_constraints);
		std::vector<std::vector<glm::int32>> batch_ids;
		for (int cloth_particle_index = 0; cloth_particle_index < graph_coloring.size(); ++cloth_particle_index) {
			unsigned int color = graph_coloring[cloth_particle_index];
			if (color + 1 > batch_ids.size()) {
				batch_ids.push_back(std::vector<glm::int32>());
				batch_ids[color].push_back(static_cast<glm::int32>(cloth_particle_index));
			}
			else {
				batch_ids[color].push_back(static_cast<glm::int32>(cloth_particle_index));
			}
		}

		for (size_t i = 0; i < batch_ids.size(); ++i) {
			batch_id_buffers_.push_back(Core::ShaderStorageBuffer::Create(batch_ids[i], batch_ids[i].size() * sizeof(glm::int32)));
		}
		
		batch_id_start_ind_ = cloth_storage_array_->AddShaderStorageBuffer(batch_id_buffers_[0]);
	}

	Core::Ref<Core::ShaderStorageArray> Cloth::GetClothStorageArray(int batch_id)
	{
		PROFILE_FUNCTION();

		cloth_storage_array_->SetShaderStorageBuffer(batch_id_start_ind_, batch_id_buffers_[batch_id]);
		return cloth_storage_array_;
	}

}