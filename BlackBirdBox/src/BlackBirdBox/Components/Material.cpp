#include "bbbpch.h"
#include "Material.h"

namespace Core {
	

	Material::Material(const Ref<Shader>& shader, const PhongLightingParameters& phong_lighting_parameters, const std::string& name) :
		shader_(shader),
		phong_lighting_parameters_(phong_lighting_parameters),
		name_(name)
	{

	}

	void Material::SetTexture(const std::string& tex_name, const Ref<Texture>& texture)
	{
		PROFILE_FUNCTION();

		if (TextureExists(tex_name)) {
			LOG_WARN("Texture already exists! You will overide texture");
		}
		
		textures_[tex_name] = texture;
	}

	Ref<Texture> Material::GetTexture(const std::string& name)
	{
		CORE_ASSERT(TextureExists(name), "Texture not found!");
		return textures_[name];
	}

	void Material::BindTextures() const
	{
		PROFILE_FUNCTION();

		for (std::pair<std::string, Ref<Texture>> pair : textures_)
		{
			pair.second->Bind();
		}
	}

	void Material::BindLightData() const
	{
		PROFILE_FUNCTION();

		// TODO(Rok Kos): Add null check
		shader_->SetFloat3("u_DiffuseColor", phong_lighting_parameters_.diffuse_color_);
		shader_->SetFloat3("u_SpecularColor", phong_lighting_parameters_.specular_color_);
		shader_->SetFloat("u_SpecularScatering", phong_lighting_parameters_.specular_scattering_);
		shader_->SetFloat3("u_AmbientColor", phong_lighting_parameters_.ambient_color_);
		shader_->SetFloat3("u_AmbientIntensity", phong_lighting_parameters_.ambient_intensity_);
	}

	bool Material::TextureExists(const std::string& name) const
	{
		PROFILE_FUNCTION();

		return textures_.find(name) != textures_.end();
	}

}