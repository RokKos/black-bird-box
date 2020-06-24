#pragma once

#include "BlackBirdBox/Primitives/Shape.h"
#include "BlackBirdBox/Components/Material.h"
#include "BlackBirdBox/Renderer/Buffers/ShaderStorageArray.h"
#include "BlackBirdBox/Renderer/Buffers/ShaderStorageBuffer.h"

namespace Core {
	class Cloth : public Shape
	{
	public:
		Cloth(unsigned int num_cloth_dimension_size, Ref<Material> material_to_render_cloth);

		Ref<ShaderStorageArray> GetClothStorageArray(int batch_id);
		size_t GetNumberOfBatchers() const { return batch_id_buffers_.size(); };

	private:
		Ref<ShaderStorageArray> cloth_storage_array_;
		std::vector<Ref<ShaderStorageBuffer>> batch_id_buffers_;
		size_t batch_id_start_ind_ = 0;
		unsigned int num_cloth_dimension_size_;
		unsigned int num_cloth_particles_;
	};
}