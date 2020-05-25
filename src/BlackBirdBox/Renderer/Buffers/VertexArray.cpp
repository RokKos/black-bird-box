#include "bbbpch.h"
#include "VertexArray.h"

#include "BlackBirdBox/Renderer/Renderer.h"
#include "Platform/OpenGL/Buffers/OpenGLVertexArray.h"

namespace Core {

	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<Platform::OpenGLVertexArray>();
		}

		CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}