#pragma once

#include "RendererAPI.h"

namespace Core {

	class RenderCommand
	{
	public:
		static void Init()
		{
			s_RendererAPI->Init();
		}

		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		static void Clear()
		{
			s_RendererAPI->Clear();
		}

		static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count = 0)
		{
			s_RendererAPI->DrawIndexed(vertexArray, count);
		}

		static void DrawPoints(const std::vector<Ref<Point>>& points)
		{
			s_RendererAPI->DrawPoints(points);
		}

		static void DispatchCompute(const ComputeShaderConfiguration& compute_shader_configuration) {
			s_RendererAPI->DispatchCompute(compute_shader_configuration);
		}

		static void WaitMemoryBarrier()
		{
			s_RendererAPI->WaitMemoryBarrier();
		}

		static void SetPolygonMode(RendererAPI::PolygonMode mode)
		{
			s_RendererAPI->SetPolygonMode(mode);
		}

		static void SetDepthFunction(RendererAPI::DepthFunction depth_function)
		{
			s_RendererAPI->SetDepthFunction(depth_function);
		}

		static void Enable(RendererAPI::RenderCapabilities capability)
		{
			s_RendererAPI->Enable(capability);
		}

		static void Disable(RendererAPI::RenderCapabilities capability)
		{
			s_RendererAPI->Disable(capability);
		}

	private:
		static Scope<RendererAPI> s_RendererAPI;
	};

}
