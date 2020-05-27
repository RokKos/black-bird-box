#include "bbbpch.h"
#include "OpenGLVertexBuffer.h"

#include <GL/glew.h>

namespace Platform {

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
	{
		glCreateBuffers(1, &renderer_id_);
		glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(Core::Vertex* vertices, uint32_t size)
	{
		glCreateBuffers(1, &renderer_id_);
		glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{
		glCreateBuffers(1, &renderer_id_);
		glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &renderer_id_);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}
}