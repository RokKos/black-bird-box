#pragma once


#include "BlackBirdBox/Core/Core.h"

#include "BlackBirdBox/Primitives/PhongLightingParamaters.h"
#include "BlackBirdBox/Renderer/Shader.h"
#include "BlackBirdBox/Renderer/Texture/Texture.h"

namespace Core {

	class Material
	{
	public:
		Material() = default;
		Material(const Ref<Shader>& shader, const PhongLightingParameters& phong_lighting_parameters = PhongLightingParameters(), const std::string& name = "material");

		const Ref<Shader>& GetShader() const { return shader_; }

		void SetTexture(const std::string& tex_name, const Ref<Texture>& texture);
		Ref<Texture> GetTexture(const std::string& tex_name);
		void BindTextures() const;
		void BindLightData() const;

		const std::string& GetName() const { return name_; }
		
		const PhongLightingParameters& GetPhongLightingParameters() const { return phong_lighting_parameters_; }
		void SetPhongLightingParameters(const PhongLightingParameters& phong_lighting_parameters) { phong_lighting_parameters_ = phong_lighting_parameters; }


	private:
		bool TextureExists(const std::string& name) const;
	private:
		std::string name_ = "material";

		Ref<Shader> shader_;
		std::unordered_map<std::string, Ref<Texture>> textures_;
		PhongLightingParameters phong_lighting_parameters_;
	};

}