#pragma once

#include "BlackBirdBox/Renderer/Buffers/ShaderStorageBuffer.h"

namespace Platform {

class OpenGLShaderStorageBuffer : public BlackBirdBox::ShaderStorageBuffer {
public:
    OpenGLShaderStorageBuffer(uint32_t size);
    OpenGLShaderStorageBuffer(const std::vector<glm::vec3>& storage_data, uint32_t size);
    OpenGLShaderStorageBuffer(const std::vector<glm::vec4>& storage_data, uint32_t size, bool persistent);
    OpenGLShaderStorageBuffer(const std::vector<glm::uvec4>& storage_data, uint32_t size);
    OpenGLShaderStorageBuffer(const std::vector<glm::mat3>& storage_data, uint32_t size);
    OpenGLShaderStorageBuffer(const std::vector<glm::mat4>& storage_data, uint32_t size);
    OpenGLShaderStorageBuffer(const std::vector<glm::int32>& storage_data, uint32_t size);
    virtual ~OpenGLShaderStorageBuffer();

    virtual uint32_t GetRendererID() const override { return m_RendererID; };
    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void SetData(const std::vector<glm::vec3>& storage_data, uint32_t size) override;
    virtual void SetData(const std::vector<glm::vec4>& storage_data, uint32_t size) override;
    virtual void SetData(const std::vector<glm::uvec4>& storage_data, uint32_t size) override;
    virtual void SetData(const std::vector<glm::mat3>& storage_data, uint32_t size) override;
    virtual void SetData(const std::vector<glm::mat4>& storage_data, uint32_t size) override;
    virtual void SetData(const std::vector<glm::int32>& storage_data, uint32_t size) override;

    virtual void SetPersistentData(const std::vector<glm::vec4>& storage_data, uint32_t size) override;
    virtual void SetPersistentDataIndex(const glm::vec4& value, size_t index) override;

    virtual std::vector<glm::vec3> GetData(uint32_t size) override;
    // virtual std::vector<glm::vec4> GetData(uint32_t size) override;

private:
    uint32_t m_RendererID;
    glm::vec4* persistent_mapped_buffer_;
};

}
