#include "bbbpch.h"
#include "FrameBuffer.h"

#include "BlackBirdBox/Renderer/Renderer.h"
#include "Platform/OpenGL/Buffers/OpenGLFrameBuffer.h"

namespace Core {
	
	Core::Ref<Core::FrameBuffer> FrameBuffer::Create(uint32_t width, uint32_t height, bool render_to_camera)
	{
		PROFILE_FUNCTION();

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<Platform::OpenGLFrameBuffer>(width, height, render_to_camera);
		}

		CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
		
	}

}