#include "bbbpch.h"
#include "ShaderStorageArray.h"

#include "BlackBirdBox/Renderer/Renderer.h"
#include "Platform/OpenGL/Buffers/OpenGLShaderStorageArray.h"

namespace Core {
	Ref<ShaderStorageArray> ShaderStorageArray::Create()
	{
		PROFILE_FUNCTION();

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<Platform::OpenGLShaderStorageArray>();
		}

		CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}