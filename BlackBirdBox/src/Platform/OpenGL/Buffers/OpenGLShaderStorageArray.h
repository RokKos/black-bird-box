#pragma once

#include "BlackBirdBox/Renderer/Buffers/ShaderStorageArray.h"

namespace Platform {

class OpenGLShaderStorageArray : public BlackBirdBox::ShaderStorageArray {
public:
    OpenGLShaderStorageArray() = default;

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual size_t AddShaderStorageBuffer(const BlackBirdBox::Ref<BlackBirdBox::ShaderStorageBuffer>& shader_storage_buffer) override;
    virtual void SetShaderStorageBuffer(int id, const BlackBirdBox::Ref<BlackBirdBox::ShaderStorageBuffer>& shader_storage_buffer) override;
    virtual const std::vector<BlackBirdBox::Ref<BlackBirdBox::ShaderStorageBuffer>>& GetShaderStorageBuffers() const override
    {
        return shader_storage_buffers_;
    }

private:
    uint32_t m_RendererID;
    std::vector<BlackBirdBox::Ref<BlackBirdBox::ShaderStorageBuffer>> shader_storage_buffers_;
};

}
