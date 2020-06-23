#pragma once

#include "BlackBirdBox/Primitives/Shape.h"
#include "BlackBirdBox/Components/Material.h"
#include "BlackBirdBox/Renderer/Buffers/ShaderStorageArray.h"

namespace Core {
	class Cloth : public Shape
	{
	public:
		Cloth(unsigned int num_cloth_dimension_size, Ref<Material> material_to_render_cloth);

		Ref<ShaderStorageArray> GetClothStorageArray() { return cloth_storage_array_; };

	private:
		Ref<ShaderStorageArray> cloth_storage_array_;
		unsigned int num_cloth_dimension_size_;
		unsigned int num_cloth_particles_;
	};
}