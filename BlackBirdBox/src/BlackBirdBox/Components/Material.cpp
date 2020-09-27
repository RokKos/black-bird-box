#include "bbbpch.h"
#include "Material.h"

#include <limits>

namespace BlackBirdBox {
Uniform::Uniform(std::string name, const std::string& type)
    : name_(std::move(name))
{
    if (type == "int") {
        type_ = UniformType::Int;
        integer_value = std::numeric_limits<int>::max();
    } else if (type == "float") {
        type_ = UniformType::Float;
        float_value = std::numeric_limits<float>::max();
    } else if (type == "vec2") {
        type_ = UniformType::Vec2;
        vec2_value = std::numeric_limits<glm::vec2>::max();
    } else if (type == "vec3") {
        type_ = UniformType::Vec3;
        vec3_value = std::numeric_limits<glm::vec3>::max();
    } else if (type == "vec4") {
        type_ = UniformType::Vec4;
        vec4_value = std::numeric_limits<glm::vec4>::max();
    } else if (type == "mat4") {
        type_ = UniformType::Mat4;
        mat4_value = std::numeric_limits<glm::mat4>::max();
    } else if (type == "sampler2D") {
        type_ = UniformType::Sampler2D;
        integer_value = std::numeric_limits<int>::max();
    } else if (type == "samplerCube") {
        type_ = UniformType::SamplerCube;
        integer_value = std::numeric_limits<int>::max();
    } else {
        CORE_ASSERT(false, "Unsuported type: %s", type.c_str());
    }
}

Material::Material(const Ref<Shader>& shader, std::string name)
    : name_(std::move(name))
    , shader_(shader)
{

    const std::string kUniformTag = "uniform";
    for (const auto& type_and_shader_source : shader->GetShaderSources()) {
        const std::string source = type_and_shader_source.second;
        size_t pos = source.find(kUniformTag);

        while (pos != std::string::npos) {

            size_t begin_uniform_type = pos + kUniformTag.length() + 1;
            size_t end_uniform_type = source.find(' ', begin_uniform_type);
            std::string uniform_type = source.substr(begin_uniform_type, end_uniform_type - begin_uniform_type);

            size_t begin_unifrom_name = end_uniform_type + 1;
            size_t end_unifrom_name = source.find(';', begin_unifrom_name);
            std::string uniform_name = source.substr(begin_unifrom_name, end_unifrom_name - begin_unifrom_name);

            uniforms_.emplace_back(Uniform(uniform_name, uniform_type));

            pos = source.find(kUniformTag, end_unifrom_name);
        }
    }
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

    for (const auto& pair : textures_) {
        pair.second->Bind();
    }
}

void Material::BindUniforms()
{
    for (const Uniform& uniform : uniforms_) {
        switch (uniform.type_) {
        case UniformType::Int:
            CORE_ASSERT(uniform.integer_value != std::numeric_limits<int>::max(), "Integer value on uniform %s was not set", uniform.name_.c_str());
            shader_->SetInt(uniform.name_, uniform.integer_value);
            break;

        case UniformType::Float:
            CORE_ASSERT(uniform.float_value != std::numeric_limits<float>::max(), "Float value on uniform %s was not set", uniform.name_.c_str());
            shader_->SetFloat(uniform.name_, uniform.float_value);
            break;
        case UniformType::Vec2:
            CORE_ASSERT(uniform.vec2_value != std::numeric_limits<glm::vec2>::max(), "Vec2 value on uniform %s was not set", uniform.name_.c_str());
            shader_->SetVec2(uniform.name_, uniform.vec2_value);
            break;

        case UniformType::Vec3:
            CORE_ASSERT(uniform.vec3_value != std::numeric_limits<glm::vec3>::max(), "Vec3 value on uniform %s was not set", uniform.name_.c_str());
            shader_->SetVec3(uniform.name_, uniform.vec3_value);
            break;

        case UniformType::Vec4:
            CORE_ASSERT(uniform.vec4_value != std::numeric_limits<glm::vec4>::max(), "Vec4 value on uniform %s was not set", uniform.name_.c_str());
            shader_->SetVec4(uniform.name_, uniform.vec4_value);
            break;

        case UniformType::Mat4:
            CORE_ASSERT(uniform.mat4_value != std::numeric_limits<glm::mat4>::max(), "Mat4 value on uniform %s was not set", uniform.name_.c_str());
            shader_->SetMat4(uniform.name_, uniform.mat4_value);
            break;

        case UniformType::Sampler2D:
            CORE_ASSERT(uniform.integer_value != std::numeric_limits<int>::max(), "Sampler2d value on uniform %s was not set", uniform.name_.c_str());
            shader_->SetInt(uniform.name_, uniform.integer_value);
            break;

        case UniformType::SamplerCube:
            CORE_ASSERT(
                uniform.integer_value != std::numeric_limits<int>::max(), "SamplerCube value on uniform %s was not set", uniform.name_.c_str());
            shader_->SetInt(uniform.name_, uniform.integer_value);
            break;

        default:
            CORE_ASSERT(false, "Unsuported type on uniform %s", uniform.name_.c_str());
        }
    }
}

const std::vector<Uniform>& Material::GetUniforms() const { return uniforms_; }

std::vector<Uniform>& Material::GetUniforms() { return uniforms_; }

const Uniform& Material::GetUniformByName(const std::string& name) const
{
    for (const Uniform& uniform : uniforms_) {
        if (uniform.name_ == name) {
            return uniform;
        }
    }

    CORE_ASSERT(false, "Uniform with name: %s was not found in shader: %s", name.c_str(), shader_->GetName().c_str());
    return uniforms_[0];
}

Uniform& Material::GetUniformByName(const std::string& name)
{
    for (Uniform& uniform : uniforms_) {
        if (uniform.name_ == name) {
            return uniform;
        }
    }

    CORE_ASSERT(false, "Uniform with name: %s was not found in shader: %s", name.c_str(), shader_->GetName().c_str());
    return uniforms_[0];
}

bool Material::HasUniform(const std::string& name)
{
    for (const Uniform& uniform : uniforms_) {
        if (uniform.name_ == name) {

            return true;
        }
    }

    return false;
}

void Material::SetUniform(const std::string& name, int integer_value)
{
    for (Uniform& uniform : uniforms_) {
        if (uniform.name_ == name) {
            uniform.integer_value = integer_value;
            return;
        }
    }

    CORE_ASSERT(false, "Uniform with name: %s was not found in shader: %s", name.c_str(), shader_->GetName().c_str());
}

void Material::SetUniform(const std::string& name, float float_value)
{
    for (Uniform& uniform : uniforms_) {
        if (uniform.name_ == name) {
            uniform.float_value = float_value;
            return;
        }
    }

    CORE_ASSERT(false, "Uniform with name: %s was not found in shader: %s", name.c_str(), shader_->GetName().c_str());
}

void Material::SetUniform(const std::string& name, glm::vec2 vec2_value)
{
    for (Uniform& uniform : uniforms_) {
        if (uniform.name_ == name) {
            uniform.vec2_value = vec2_value;
            return;
        }
    }

    CORE_ASSERT(false, "Uniform with name: %s was not found in shader: %s", name.c_str(), shader_->GetName().c_str());
}

void Material::SetUniform(const std::string& name, glm::vec3 vec3_value)
{
    for (Uniform& uniform : uniforms_) {
        if (uniform.name_ == name) {
            uniform.vec3_value = vec3_value;
            return;
        }
    }

    CORE_ASSERT(false, "Uniform with name: %s was not found in shader: %s", name.c_str(), shader_->GetName().c_str());
}

void Material::SetUniform(const std::string& name, glm::vec4 vec4_value)
{
    for (Uniform& uniform : uniforms_) {
        if (uniform.name_ == name) {
            uniform.vec4_value = vec4_value;
            return;
        }
    }

    CORE_ASSERT(false, "Uniform with name: %s was not found in shader: %s", name.c_str(), shader_->GetName().c_str());
}

void Material::SetUniform(const std::string& name, glm::mat4 mat4_value)
{
    for (Uniform& uniform : uniforms_) {
        if (uniform.name_ == name) {
            uniform.mat4_value = mat4_value;
            return;
        }
    }

    CORE_ASSERT(false, "Uniform with name: %s was not found in shader: %s", name.c_str(), shader_->GetName().c_str());
}

bool Material::TextureExists(const std::string& name) const
{
    PROFILE_FUNCTION();

    return textures_.find(name) != textures_.end();
}

}
