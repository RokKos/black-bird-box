#pragma once

#include "BlackBirdBox/Components/Material.h"
#include "BlackBirdBox/Primitives/ComputeShaderSimulationConfiguration.h"
#include "BlackBirdBox/Primitives/LightSource.h"
#include "Buffers/FrameBuffer.h"
#include "Buffers/ShaderStorageArray.h"
#include "Buffers/VertexArray.h"
#include "Cameras/Camera.h"
#include "CubeMap/CubeMap.h"
#include "RendererAPI.h"
#include "Shader.h"

namespace Core {

class Renderer {
public:
    static void Init();
    static void Shutdown();

    static void OnWindowResize(uint32_t width, uint32_t height);

    static void BeginScene(Camera& camera, std::vector<Ref<LightSource>> light_sources);
    static void EndScene();
    static void Submit(const Ref<Material>& material, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
    static void Submit(const Ref<Material>& material, const Ref<CubeMap>& cube_map, const Ref<VertexArray>& vertexArray,
        const glm::mat4& transform = glm::mat4(1.0f));
    static void Submit(const Ref<FrameBuffer>& frame_buffer, const Ref<Material>& material, const Ref<VertexArray>& vertexArray,
        const glm::mat4& transform = glm::mat4(1.0f));
    static void DispatchComputeShader(const Ref<Material>& material, const Ref<ShaderStorageArray>& shader_storage_array,
        const ComputeShaderConfiguration& compute_shader_configuration);
    static void DrawPoints(const std::vector<Ref<Point>>& points);

    inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

private:
    // TODO(Rok Kos): Rething this Scene Data... maybe replace with Scene
    struct SceneData {

        glm::mat4 ViewProjectionMatrix;
        glm::mat4 ViewMatrix;
        glm::mat4 ProjectionMatrix;
        glm::vec3 camera_position_;
        std::vector<Ref<LightSource>> light_sources_;
    };

    static Scope<SceneData> s_SceneData;
};
}
