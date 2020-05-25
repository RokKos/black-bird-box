#pragma once

#include "BlackBirdBox/Primitives/ModelData.h"

#include "tiny_obj_loader.h"

namespace Core {

	class ModelLoader {
	public:
		static ModelData LoadModel(const std::string& model_path);
	};

}