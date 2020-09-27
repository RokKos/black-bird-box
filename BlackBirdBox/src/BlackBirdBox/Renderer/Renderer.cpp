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

void Renderer::Shutdown() { PROFILE_FUNCTION(); }

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

void Renderer::EndScene() { PROFILE_FUNCTION(); }

void Renderer::Submit(const Ref<Material>& material, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
{
    PROFILE_FUNCTION();

    // TODO(Rok Kos): This is only temporary
    material->SetUniform("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
    material->SetUniform("u_Transform", transform);
    if (material->HasUniform("u_CameraPosition")) {
        material->SetUniform("u_CameraPosition", s_SceneData->camera_position_);
    }

    if (material->HasUniform("u_LightPosition")) {
        // TODO(Rok Kos): Temporary, find solution for multiple lights
        for (auto light_source : s_SceneData->light_sources_) {
            material->SetUniform("u_LightPosition", light_source->GetPosition());
            material->SetUniform("u_LightDirection", light_source->GetDirection());
            material->SetUniform("u_LightIntensity", light_source->GetIntensity());
            material->SetUniform("u_LightColor", light_source->GetColor());
        }
    }

    material->BindTextures();

    const Ref<Shader>& shader = material->GetShader();
    shader->Bind();
    material->BindUniforms();

    vertexArray->Bind();
    RenderCommand::DrawIndexed(vertexArray);
}

void Renderer::Submit(const Ref<Material>& material, const Ref<CubeMap>& cube_map, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
{
    PROFILE_FUNCTION();

    // TODO(Rok Kos): This is only temporary
    glm::mat4 view_matrix = glm::mat4(glm::mat3(s_SceneData->ViewMatrix));
    material->SetUniform("u_ViewProjection", s_SceneData->ProjectionMatrix * view_matrix);
    material->SetUniform("u_Transform", transform);

    const Ref<Shader>& shader = material->GetShader();

    // TODO(Rok Kos): This is only temporary
    RenderCommand::SetDepthFunction(RendererAPI::DepthFunction::LEQUAL);
    shader->Bind();
    material->BindUniforms();

    vertexArray->Bind();
    cube_map->Bind();
    RenderCommand::DrawIndexed(vertexArray);
    // TODO(Rok Kos): This is only temporary
    RenderCommand::SetDepthFunction(RendererAPI::DepthFunction::LESS);
}

void Renderer::Submit(
    const Ref<FrameBuffer>& frame_buffer, const Ref<Material>& material, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
{
    PROFILE_FUNCTION();

    // TODO(Rok Kos): This is only temporary
    material->SetUniform("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
    material->SetUniform("u_Transform", transform);

    const Ref<Shader>& shader = material->GetShader();
    shader->Bind();
    material->BindUniforms();

    vertexArray->Bind();

    RenderCommand::Disable(RendererAPI::RenderCapabilities::DEPTH_TEST);

    frame_buffer->GetTextureAttachment(FrameBufferAttachments::COLOR_ATTACHMENT0)->Bind();

    RenderCommand::DrawIndexed(vertexArray);

    RenderCommand::Enable(RendererAPI::RenderCapabilities::DEPTH_TEST);
}

void Renderer::DispatchComputeShader(const Ref<Material>& material, const Ref<ShaderStorageArray>& shader_storage_array,
    const ComputeShaderConfiguration& compute_shader_configuration)
{
    PROFILE_FUNCTION();

    const Ref<Shader>& shader = material->GetShader();
    shader->Bind();
    material->BindUniforms();
    shader_storage_array->Bind();

    RenderCommand::DispatchCompute(compute_shader_configuration);
    RenderCommand::WaitMemoryBarrier();
}

void Renderer::DrawPoints(const std::vector<Ref<Point>>& points)
{
    PROFILE_FUNCTION();

    RenderCommand::DrawPoints(points);
}

}
