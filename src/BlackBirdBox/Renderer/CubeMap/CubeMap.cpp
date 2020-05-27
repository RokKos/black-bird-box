#include "bbbpch.h"
#include "CubeMap.h"

#include "BlackBirdBox/Renderer/Renderer.h"
#include "Platform/OpenGL/CubeMap/OpenGLCubeMap.h"

namespace Core {
	
	Ref<CubeMap> CubeMap::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<Platform::OpenGLCubeMap>(path);
		}

		CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}



