#include "bbbpch.h"
#include "Renderer.h"

#include "RenderCommand.h"

namespace Core {

	Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();

	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::Shutdown()
	{

	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(Camera& camera, std::vector<Ref<LightSource>> light_sources)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
		s_SceneData->camera_position_ = camera.GetPosition();
		s_SceneData->light_sources_ = light_sources;
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const Ref<Material>& material, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
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

	void Renderer::DispatchComputeShader(const Ref<Shader> shader, const Ref<ShaderStorageArray>& shader_storage_array, const ComputeShaderConfiguration& compute_shader_configuration)
	{
		shader_storage_array->Bind();
		shader->Bind();
		RenderCommand::DispatchCompute(compute_shader_configuration);
		RenderCommand::WaitMemoryBarrier();

	}

	void Renderer::DrawPoints(const std::vector<Ref<Point>>& points)
	{
		RenderCommand::DrawPoints(points);
	}

}