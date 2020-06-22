#include "bbbpch.h"
#include "ShaderStorageBuffer.h"

#include "BlackBirdBox/Renderer/Renderer.h"
#include "Platform/OpenGL/Buffers/OpenGLShaderStorageBuffer.h"

namespace Core {


	Ref<ShaderStorageBuffer> ShaderStorageBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<Platform::OpenGLShaderStorageBuffer>(size);
		}

		CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<ShaderStorageBuffer> ShaderStorageBuffer::Create(const std::vector<glm::vec3>& storage_data, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<Platform::OpenGLShaderStorageBuffer>(storage_data, size);
		}

		CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<ShaderStorageBuffer> ShaderStorageBuffer::Create(const std::vector<glm::vec4>& storage_data, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<Platform::OpenGLShaderStorageBuffer>(storage_data, size);
		}

		CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<ShaderStorageBuffer> ShaderStorageBuffer::Create(const std::vector<glm::mat3>& storage_data, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<Platform::OpenGLShaderStorageBuffer>(storage_data, size);
		}

		CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<ShaderStorageBuffer> ShaderStorageBuffer::Create(const std::vector<glm::mat4>& storage_data, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<Platform::OpenGLShaderStorageBuffer>(storage_data, size);
		}

		CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}


	Ref<ShaderStorageBuffer> ShaderStorageBuffer::Create(const std::vector<glm::int32>& storage_data, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<Platform::OpenGLShaderStorageBuffer>(storage_data, size);
		}

		CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}



}