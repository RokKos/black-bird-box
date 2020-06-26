#include "bbbpch.h"
#include "Texture2D.h"

#include "BlackBirdBox/Renderer/Renderer.h"
#include "Platform/OpenGL/Texture/OpenGLTexture2D.h"

namespace Core {

	Ref<Texture2D> Texture2D::Create(const Texture2DSpecification& specification)
	{
		PROFILE_FUNCTION();

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<Platform::OpenGLTexture2D>(specification);
		}

		CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path, const Texture2DSpecification& specification)
	{
		PROFILE_FUNCTION();

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<Platform::OpenGLTexture2D>(path, specification);
		}

		CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}