#pragma once

#include "BlackBirdBox/Renderer/RendererAPI.h"

#include <glm/glm.hpp>
#include <GL/glew.h>

namespace Platform {

	class OpenGLRendererAPI : public Core::RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Core::Ref<Core::VertexArray>& vertexArray, uint32_t indexCount = 0) override;
		virtual void DrawPoints(const std::vector<Core::Ref<Core::Point>>& points) override;

		virtual void DispatchCompute(const Core::ComputeShaderConfiguration& compute_shader_configuration) override;
		virtual void WaitMemoryBarrier() override;
		virtual void SetDepthFunction(Core::RendererAPI::DepthFunction depth_function) override;
		virtual void SetPolygonMode(Core::RendererAPI::PolygonMode mode) override;
		virtual void Enable(Core::RendererAPI::RenderCapabilities capability) override;
		virtual void Disable(Core::RendererAPI::RenderCapabilities capability) override;

	private:
		GLenum OpenGLRenderCapabilities(Core::RendererAPI::RenderCapabilities capability) const;
	};
}