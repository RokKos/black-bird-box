#include "bbbpch.h"
#include "IndexBuffer.h"

#include "BlackBirdBox/Renderer/Renderer.h"
#include "Platform/OpenGL/Buffers/OpenGLIndexBuffer.h"

namespace BlackBirdBox {
Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
{
    PROFILE_FUNCTION();

    switch (Renderer::GetAPI()) {
    case RendererAPI::API::None:
        CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return CreateRef<Platform::OpenGLIndexBuffer>(indices, size);
    }

    CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}
}
