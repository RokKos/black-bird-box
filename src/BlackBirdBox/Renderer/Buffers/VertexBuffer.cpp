#include "bbbpch.h"
#include "VertexBuffer.h"

#include "BlackBirdBox/Renderer/Renderer.h"
#include "Platform/OpenGL/Buffers/OpenGLVertexBuffer.h"

namespace Core {
	Ref<VertexBuffer> VertexBuffer::Create(Vertex* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<Platform::OpenGLVertexBuffer>(vertices, size);
		}

		CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(glm::vec3* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<Platform::OpenGLVertexBuffer>(vertices, size);
		}

		CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<Platform::OpenGLVertexBuffer>(vertices, size);
		}

		CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::CreateExistingBuffer(uint32_t renderer_id)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<Platform::OpenGLVertexBuffer>(renderer_id);
		}

		CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}