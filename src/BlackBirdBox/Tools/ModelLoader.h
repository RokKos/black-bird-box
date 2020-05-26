#pragma once

#include "BlackBirdBox/Primitives/ModelData.h"

namespace Core {

	class ModelLoader {
	public:
		static ModelData LoadModel(const std::string& model_path);
	};

}