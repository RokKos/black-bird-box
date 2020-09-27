#include "bbbpch.h"
#include "GraphicsContext.h"

#include "Platform/OpenGL/OpenGLContext.h"
#include "Renderer.h"

namespace BlackBirdBox {

Scope<GraphicsContext> GraphicsContext::Create(void* window)
{
    PROFILE_FUNCTION();

    switch (Renderer::GetAPI()) {
    case RendererAPI::API::None:
        CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return CreateScope<Platform::OpenGLContext>(static_cast<GLFWwindow*>(window));
    }

    CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

}
