#include "bbbpch.h"
#include "Texture2D.h"

#include "BlackBirdBox/Renderer/Renderer.h"
#include "Platform/OpenGL/Texture/OpenGLTexture2D.h"

namespace Core {

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		PROFILE_FUNCTION();

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<Platform::OpenGLTexture2D>(width, height);
		}

		CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		PROFILE_FUNCTION();

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<Platform::OpenGLTexture2D>(path);
		}

		CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}