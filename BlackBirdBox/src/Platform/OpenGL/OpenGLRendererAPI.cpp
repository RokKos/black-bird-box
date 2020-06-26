#include "bbbpch.h"
#include "OpenGLRendererAPI.h"

#include <GL/glew.h>

namespace Platform {

	void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         LOG_CRITICAL(message); return;
		case GL_DEBUG_SEVERITY_MEDIUM:       LOG_ERROR(message); return;
		case GL_DEBUG_SEVERITY_LOW:          LOG_WARN(message); return;
		case GL_DEBUG_SEVERITY_NOTIFICATION: LOG_TRACE(message); return;
		}

		CORE_ASSERT(false, "Unknown severity level!");
	}

	void OpenGLRendererAPI::Init()
	{
		PROFILE_FUNCTION();

		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		PROFILE_FUNCTION();

		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		PROFILE_FUNCTION();

		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		PROFILE_FUNCTION();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Core::Ref<Core::VertexArray>& vertexArray, uint32_t indexCount /*= 0*/)
	{
		PROFILE_FUNCTION();

		uint32_t count = indexCount == 0 ? vertexArray->GetIndexBuffer()->GetCount() : indexCount;  // TODO(Rok Kos): Validate on more cases that this is correct fix
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void OpenGLRendererAPI::DrawPoints(const std::vector<Core::Ref<Core::Point>>& points)
	{
		PROFILE_FUNCTION();

		glEnable(GL_POINT_SMOOTH);
		// TODO(Rok Kos): Figure out how to get size and color for all points without having to do multiple glBegin and glEnd
		glPointSize(10.0f);
		glColor3f(1.0f, 0.0f, 0.0f);

		glBegin(GL_POINTS);
		for (auto point : points) {
			glm::vec3 position = point->GetPosition();
			glVertex3f(position.x, position.y, position.z);
		}
		glEnd();
		glDisable(GL_POINT_SMOOTH);
	}

	void OpenGLRendererAPI::DispatchCompute(const Core::ComputeShaderConfiguration& compute_shader_configuration)
	{
		PROFILE_FUNCTION();

		glDispatchComputeGroupSizeARB(compute_shader_configuration.GetWorkGroupSize()[0], compute_shader_configuration.GetWorkGroupSize()[1], compute_shader_configuration.GetWorkGroupSize()[2],
			compute_shader_configuration.GetLocalGroupSize()[0], compute_shader_configuration.GetLocalGroupSize()[1], compute_shader_configuration.GetLocalGroupSize()[2]);
	}

	void OpenGLRendererAPI::WaitMemoryBarrier()
	{
		PROFILE_FUNCTION();

		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	}

	void OpenGLRendererAPI::SetDepthFunction(Core::RendererAPI::DepthFunction depth_function)
	{
		PROFILE_FUNCTION();

		switch (depth_function)
		{
		case Core::RendererAPI::DepthFunction::NEVER:
			glDepthFunc(GL_NEVER);
			break;
		case Core::RendererAPI::DepthFunction::LESS:
			glDepthFunc(GL_LESS);
			break;
		case Core::RendererAPI::DepthFunction::EQUAL:
			glDepthFunc(GL_EQUAL);
			break;
		case Core::RendererAPI::DepthFunction::LEQUAL:
			glDepthFunc(GL_LEQUAL);
			break;
		case Core::RendererAPI::DepthFunction::GREATER:
			glDepthFunc(GL_GREATER);
			break;
		case Core::RendererAPI::DepthFunction::NOTEQUAL:
			glDepthFunc(GL_NOTEQUAL);
			break;
		case Core::RendererAPI::DepthFunction::GEQUAL:
			glDepthFunc(GL_GEQUAL);
			break;
		case Core::RendererAPI::DepthFunction::ALWAYS:
			glDepthFunc(GL_ALWAYS);
			break;
		default:
			CORE_ASSERT(false, "Depth function unknown")
				break;
		}
	}

	void OpenGLRendererAPI::SetPolygonMode(PolygonMode mode)
	{
		PROFILE_FUNCTION();

		switch (mode)
		{
		case PolygonMode::POINT:
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			break;
		case PolygonMode::LINE:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		case PolygonMode::FILL:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		default:
			break;
		}
	}

}