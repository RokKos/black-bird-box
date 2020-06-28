#pragma once

#include "BlackBirdBox/Primitives/ModelData.h"

namespace Core {

	class ModelLibrary
	{
	public:
		void Add(const std::string& name, const ModelData& model);
		const ModelData& Load(const std::string& filepath);
		const ModelData& Load(const std::string& name, const std::string& filepath);

		const ModelData& Get(const std::string& name);

		bool Exists(const std::string& name) const;

	private:
		ModelData LoadModel(const std::string& model_path);
	private:
		std::unordered_map<std::string, ModelData> models_;
	};

}