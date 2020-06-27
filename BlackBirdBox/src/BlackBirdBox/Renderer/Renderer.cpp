#include "bbbpch.h"
#include "Renderer.h"

#include "RenderCommand.h"

namespace Core {

	Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();

	void Renderer::Init()
	{
		PROFILE_FUNCTION();

		RenderCommand::Init();
	}

	void Renderer::Shutdown()
	{
		PROFILE_FUNCTION();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		PROFILE_FUNCTION();

		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(Camera& camera, std::vector<Ref<LightSource>> light_sources)
	{
		PROFILE_FUNCTION();

		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
		s_SceneData->ViewMatrix = camera.GetViewMatrix();
		s_SceneData->ProjectionMatrix = camera.GetProjectionMatrix();
		s_SceneData->camera_position_ = camera.GetPosition();
		s_SceneData->light_sources_ = light_sources;
	}

	void Renderer::EndScene()
	{
		PROFILE_FUNCTION();
	}

	void Renderer::Submit(const Ref<Material>& material, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		PROFILE_FUNCTION();

		material->BindTextures();

		Ref<Shader> shader = material->GetShader();
		shader->Bind();
		shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		shader->SetFloat3("u_CameraPosition", s_SceneData->camera_position_);
		shader->SetMat4("u_Transform", transform);

		material->BindLightData();
		// TODO(Rok Kos): Temporary, find solution for multiple lights
		for (auto light_source : s_SceneData->light_sources_)
		{
			shader->SetFloat3("u_LightPosition", light_source->GetPosition());
			shader->SetFloat3("u_LightDirection", light_source->GetDirection());
			shader->SetFloat3("u_LightIntensity", light_source->GetIntensity());
			shader->SetFloat3("u_LightColor", light_source->GetColor());
		}

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<CubeMap>& cube_map, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		PROFILE_FUNCTION();

		// TODO(Rok Kos): This is only temporary
		RenderCommand::SetDepthFunction(RendererAPI::DepthFunction::LEQUAL);
		shader->Bind();

		// TODO(Rok Kos): This is only temporary
		glm::mat4 view_matrix = glm::mat4(glm::mat3(s_SceneData->ViewMatrix));
		shader->SetMat4("u_ViewProjection", s_SceneData->ProjectionMatrix * view_matrix);
		shader->SetMat4("u_Transform", transform);

		vertexArray->Bind();
		cube_map->Bind();
		RenderCommand::DrawIndexed(vertexArray);
		// TODO(Rok Kos): This is only temporary
		RenderCommand::SetDepthFunction(RendererAPI::DepthFunction::LESS);
	}

	void Renderer::Submit(Ref<FrameBuffer> frame_buffer, const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform /*= glm::mat4(1.0f)*/)
	{
		PROFILE_FUNCTION();

		shader->Bind();
		shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		shader->SetMat4("u_Transform", transform);

		vertexArray->Bind();

		RenderCommand::Disable(RendererAPI::RenderCapabilities::DEPTH_TEST);

		frame_buffer->GetTextureColorAttachment()->Bind();
		
		RenderCommand::DrawIndexed(vertexArray);

		RenderCommand::Enable(RendererAPI::RenderCapabilities::DEPTH_TEST);
	}

	void Renderer::DispatchComputeShader(const Ref<Shader> shader, const Ref<ShaderStorageArray>& shader_storage_array, const ComputeShaderConfiguration& compute_shader_configuration, const ComputeShaderSimulationConfiguration& compute_shader_simulation_configuration)
	{
		PROFILE_FUNCTION();

		shader_storage_array->Bind();
		shader->Bind();
		shader->SetFloat3("u_Gravity", compute_shader_simulation_configuration.GetGravity());
		shader->SetFloat("u_DeltaTime", compute_shader_simulation_configuration.GetDeltaTime());
		shader->SetInt("u_Itterations", compute_shader_simulation_configuration.GetItterations());
		RenderCommand::DispatchCompute(compute_shader_configuration);
		RenderCommand::WaitMemoryBarrier();

	}

	void Renderer::DrawPoints(const std::vector<Ref<Point>>& points)
	{
		PROFILE_FUNCTION();

		RenderCommand::DrawPoints(points);
	}

}