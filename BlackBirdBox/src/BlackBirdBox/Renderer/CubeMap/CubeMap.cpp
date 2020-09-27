#include "bbbpch.h"
#include "CubeMap.h"

#include "BlackBirdBox/Renderer/Renderer.h"
#include "Platform/OpenGL/CubeMap/OpenGLCubeMap.h"

namespace BlackBirdBox {

Ref<CubeMap> CubeMap::Create(const std::string& path)
{
    PROFILE_FUNCTION();

    switch (Renderer::GetAPI()) {
    case RendererAPI::API::None:
        CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return CreateRef<Platform::OpenGLCubeMap>(path);
    }

    CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

}
