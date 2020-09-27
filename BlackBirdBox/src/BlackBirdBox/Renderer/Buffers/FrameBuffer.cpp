#include "bbbpch.h"
#include "FrameBuffer.h"

#include "BlackBirdBox/Renderer/Renderer.h"
#include "Platform/OpenGL/Buffers/OpenGLFrameBuffer.h"

namespace BlackBirdBox {

BlackBirdBox::Ref<BlackBirdBox::FrameBuffer> FrameBuffer::Create(const FramebufferSpecification& specification)
{
    PROFILE_FUNCTION();

    switch (Renderer::GetAPI()) {
    case RendererAPI::API::None:
        CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return CreateRef<Platform::OpenGLFrameBuffer>(specification);
    }

    CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

}
