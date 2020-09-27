#pragma once

#include "BlackBirdBox/Core/Core.h"

#include "BlackBirdBox/Primitives/PhongLightingParamaters.h"
#include "BlackBirdBox/Renderer/Shader.h"
#include "BlackBirdBox/Renderer/Texture/Texture.h"

namespace Core {
enum class UniformType { None = 0, Int = 1, Float = 2, Vec2 = 3, Vec3 = 4, Vec4 = 5, Mat4 = 6, Sampler2D = 7, SamplerCube = 8 };

struct Uniform {
    Uniform() = default;

    Uniform(std::string name, const std::string& type);

    union {
        int integer_value;
        float float_value;
        glm::vec2 vec2_value;
        glm::vec3 vec3_value;
        glm::vec4 vec4_value;
        glm::mat4 mat4_value;
    };
    std::string name_ = "Uniform";
    UniformType type_ = UniformType::None;
};

class Material {
public:
    Material() = default;
    Material(const Ref<Shader>& shader, std::string name = "material");

    const std::string& GetName() const { return name_; }
    [[nodiscard]] const Ref<Shader>& GetShader() const { return shader_; }

    void SetTexture(const std::string& tex_name, const Ref<Texture>& texture);
    [[nodiscard]] Ref<Texture> GetTexture(const std::string& tex_name);
    void BindTextures() const;

    void BindUniforms();
    [[nodiscard]] const std::vector<Uniform>& GetUniforms() const;
    [[nodiscard]] std::vector<Uniform>& GetUniforms();
    [[nodiscard]] const Uniform& GetUniformByName(const std::string& name) const;
    [[nodiscard]] Uniform& GetUniformByName(const std::string& name);
    [[nodiscard]] bool HasUniform(const std::string& name);
    void SetUniform(const std::string& name, int integer_value);
    void SetUniform(const std::string& name, float float_value);
    void SetUniform(const std::string& name, glm::vec2 vec2_value);
    void SetUniform(const std::string& name, glm::vec3 vec3_value);
    void SetUniform(const std::string& name, glm::vec4 vec4_value);
    void SetUniform(const std::string& name, glm::mat4 mat4_value);

private:
    bool TextureExists(const std::string& name) const;

private:
    std::string name_ = "material";

    Ref<Shader> shader_;
    std::vector<Uniform> uniforms_;
    std::unordered_map<std::string, Ref<Texture>> textures_;
};

}
