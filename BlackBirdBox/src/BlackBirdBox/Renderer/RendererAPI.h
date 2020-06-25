#pragma once

#include "BlackBirdBox/Core/Core.h"
#include "Buffers/VertexArray.h"
#include "BlackBirdBox/Primitives/Point.h"
#include "BlackBirdBox/Primitives/ComputeShaderConfiguration.h"

#include <glm/glm.hpp>

namespace Core {

	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};

		enum class DepthFunction
		{
			NEVER = 0,
			LESS = 1,
			EQUAL = 2,
			LEQUAL = 3,
			GREATER = 4,
			NOTEQUAL = 5,
			GEQUAL = 6,
			ALWAYS = 7
		};

		enum class PolygonMode
		{
			POINT = 0,
			LINE = 1,
			FILL = 2
		};

	public:
		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
		virtual void DrawPoints(const std::vector<Ref<Point>>& points) = 0;
		virtual void DispatchCompute(const ComputeShaderConfiguration& compute_shader_configuration) = 0;
		virtual void WaitMemoryBarrier() = 0;
		virtual void SetDepthFunction(DepthFunction depth_function) = 0;
		virtual void SetPolygonMode(PolygonMode mode) = 0;

		inline static API GetAPI() { return s_API; }
		static Scope<RendererAPI> Create();

	private:
		static API s_API;
	};

}