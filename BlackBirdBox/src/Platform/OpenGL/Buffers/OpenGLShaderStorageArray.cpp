#include "bbbpch.h"
#include "OpenGLShaderStorageArray.h"

#include <GL/glew.h>

namespace Platform {
	void OpenGLShaderStorageArray::Bind() const
	{
		PROFILE_FUNCTION();

		for (size_t i = 0; i < shader_storage_buffers_.size(); i++)
		{
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, i, shader_storage_buffers_[i]->GetRendererID());
		}
	
	}

	void OpenGLShaderStorageArray::Unbind() const
	{
		PROFILE_FUNCTION();

		for (size_t i = 0; i < shader_storage_buffers_.size(); i++)
		{
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, i, 0);
		}
	}

	size_t OpenGLShaderStorageArray::AddShaderStorageBuffer(const Core::Ref<Core::ShaderStorageBuffer>& shader_storage_buffer)
	{
		PROFILE_FUNCTION();

		shader_storage_buffers_.push_back(shader_storage_buffer);
		return shader_storage_buffers_.size() - 1;
	}

	void OpenGLShaderStorageArray::SetShaderStorageBuffer(int id, const Core::Ref<Core::ShaderStorageBuffer>& shader_storage_buffer)
	{
		PROFILE_FUNCTION();

		CORE_ASSERT(id < shader_storage_buffers_.size(), "SetShaderStorageBuffer id is larger than size of shader_storage_buffers_");
		shader_storage_buffers_[id] = shader_storage_buffer;
	}

}