#pragma once

#include "BlackBirdBox/Renderer/Shader.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace Platform {

class OpenGLShader : public Core::Shader {
public:
    OpenGLShader(const std::string& filepath);
    OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
    virtual ~OpenGLShader();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void SetInt(const std::string& name, int value) override;
    virtual void SetIntArray(const std::string& name, int* values, uint32_t count) override;
    virtual void SetFloat(const std::string& name, float value) override;
    virtual void SetVec2(const std::string& name, const glm::vec2& value) override;
    virtual void SetVec3(const std::string& name, const glm::vec3& value) override;
    virtual void SetVec4(const std::string& name, const glm::vec4& value) override;
    virtual void SetMat4(const std::string& name, const glm::mat4& value) override;

    virtual const std::string& GetName() const override { return name_; }
    virtual const std::unordered_map<GLenum, std::string>& GetShaderSources() const override { return shader_sources_; }

    void UploadUniformInt(const std::string& name, int value);
    void UploadUniformIntArray(const std::string& name, int* values, uint32_t count);

    void UploadUniformFloat(const std::string& name, float value);
    void UploadUniformVec2(const std::string& name, const glm::vec2& value);
    void UploadUniformVec3(const std::string& name, const glm::vec3& value);
    void UploadUniformVec4(const std::string& name, const glm::vec4& value);

    void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
    void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

private:
    std::string ReadFile(const std::string& filepath);
    std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
    void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);

    GLint GetUniformLocation(const std::string& name);

private:
    uint32_t renderer_id_;
    std::string name_;
    std::unordered_map<GLenum, std::string> shader_sources_;
    std::unordered_map<std::string, GLint> uniform_location_cache_;
};

}
