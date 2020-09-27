#pragma once

#include "BlackBirdBox/Renderer/RendererAPI.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace Platform {

class OpenGLRendererAPI : public BlackBirdBox::RendererAPI {
public:
    virtual void Init() override;
    virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
    virtual void SetClearColor(const glm::vec4& color) override;
    virtual void Clear() override;

    virtual void DrawIndexed(const BlackBirdBox::Ref<BlackBirdBox::VertexArray>& vertexArray, uint32_t indexCount = 0) override;
    virtual void DrawPoints(const std::vector<BlackBirdBox::Ref<BlackBirdBox::Point>>& points) override;

    virtual void DispatchCompute(const BlackBirdBox::ComputeShaderConfiguration& compute_shader_configuration) override;
    virtual void WaitMemoryBarrier() override;
    virtual void SetDepthFunction(BlackBirdBox::RendererAPI::DepthFunction depth_function) override;
    virtual void SetPolygonMode(BlackBirdBox::RendererAPI::PolygonMode mode) override;
    virtual void Enable(BlackBirdBox::RendererAPI::RenderCapabilities capability) override;
    virtual void Disable(BlackBirdBox::RendererAPI::RenderCapabilities capability) override;

private:
    GLenum OpenGLRenderCapabilities(BlackBirdBox::RendererAPI::RenderCapabilities capability) const;
};
}
