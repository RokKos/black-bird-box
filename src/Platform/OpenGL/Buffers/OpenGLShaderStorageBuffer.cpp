#include "bbbpch.h"
#include "OpenGLShaderStorageBuffer.h"

#include <GL/glew.h>

namespace Platform {

	OpenGLShaderStorageBuffer::OpenGLShaderStorageBuffer(uint32_t size)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
		glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLShaderStorageBuffer::OpenGLShaderStorageBuffer(const std::vector<glm::vec3>& storage_data, uint32_t size)
	{
		glCreateBuffers(1, &m_RendererID);
		SetData(storage_data, size);
	}

	OpenGLShaderStorageBuffer::OpenGLShaderStorageBuffer(const std::vector<glm::vec4>& storage_data, uint32_t size)
	{
		glCreateBuffers(1, &m_RendererID);
		SetData(storage_data, size);
	}

	OpenGLShaderStorageBuffer::~OpenGLShaderStorageBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLShaderStorageBuffer::Bind() const
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
	}

	void OpenGLShaderStorageBuffer::Unbind() const
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	void OpenGLShaderStorageBuffer::SetData(const std::vector<glm::vec3>& storage_data, uint32_t size)
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
		glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, GL_STATIC_DRAW);

		GLint buffer_mask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;

		glm::vec3* mapped_buffer = reinterpret_cast<glm::vec3*>(
			glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, size, buffer_mask));

		std::copy(storage_data.begin(), storage_data.end(), mapped_buffer);


		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	}

	void OpenGLShaderStorageBuffer::SetData(const std::vector<glm::vec4>& storage_data, uint32_t size)
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
		glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, GL_STATIC_DRAW);

		GLint buffer_mask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;

		glm::vec4* mapped_buffer = reinterpret_cast<glm::vec4*>(
			glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, size, buffer_mask));

		std::copy(storage_data.begin(), storage_data.end(), mapped_buffer);


		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	}

	std::vector<glm::vec3> OpenGLShaderStorageBuffer::GetData(uint32_t size)
	{
		// TODO(Rok Kos): Improve this function not to copy whole buffer
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID); 
		GLint buffer_mask = GL_MAP_READ_BIT;

		glm::vec3* mapped_buffer = reinterpret_cast<glm::vec3*>(
			glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, size, buffer_mask));

		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

		return std::vector<glm::vec3>(mapped_buffer, mapped_buffer + (size / sizeof(glm::vec3)));
	}

	/*std::vector<glm::vec4> OpenGLShaderStorageBuffer::GetData(uint32_t size)
	{
		// TODO(Rok Kos): Improve this function not to copy whole buffer
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
		GLint buffer_mask = GL_MAP_READ_BIT;

		glm::vec4* mapped_buffer = reinterpret_cast<glm::vec4*>(
			glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, size, buffer_mask));

		return std::vector<glm::vec4>(mapped_buffer, mapped_buffer + (size / sizeof(glm::vec4)));
	}*/

}