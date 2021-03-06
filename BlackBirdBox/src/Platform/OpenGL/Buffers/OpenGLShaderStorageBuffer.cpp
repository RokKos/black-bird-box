﻿#include "bbbpch.h"
#include "OpenGLShaderStorageBuffer.h"

#include <GL/glew.h>

namespace Platform {

OpenGLShaderStorageBuffer::OpenGLShaderStorageBuffer(uint32_t size)
{
    PROFILE_FUNCTION();

    glCreateBuffers(1, &m_RendererID);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

OpenGLShaderStorageBuffer::OpenGLShaderStorageBuffer(const std::vector<glm::vec3>& storage_data, uint32_t size)
{
    PROFILE_FUNCTION();

    glCreateBuffers(1, &m_RendererID);
    SetData(storage_data, size);
}

OpenGLShaderStorageBuffer::OpenGLShaderStorageBuffer(const std::vector<glm::vec4>& storage_data, uint32_t size, bool persistent)
{
    PROFILE_FUNCTION();

    glCreateBuffers(1, &m_RendererID);
    if (persistent) {
        SetPersistentData(storage_data, size);
    } else {
        SetData(storage_data, size);
    }
}

OpenGLShaderStorageBuffer::OpenGLShaderStorageBuffer(const std::vector<glm::uvec4>& storage_data, uint32_t size)
{
    PROFILE_FUNCTION();

    PROFILE_FUNCTION();

    glCreateBuffers(1, &m_RendererID);
    SetData(storage_data, size);
}

OpenGLShaderStorageBuffer::OpenGLShaderStorageBuffer(const std::vector<glm::mat3>& storage_data, uint32_t size)
{
    PROFILE_FUNCTION();

    glCreateBuffers(1, &m_RendererID);
    SetData(storage_data, size);
}

OpenGLShaderStorageBuffer::OpenGLShaderStorageBuffer(const std::vector<glm::int32>& storage_data, uint32_t size)
{
    PROFILE_FUNCTION();

    glCreateBuffers(1, &m_RendererID);
    SetData(storage_data, size);
}

OpenGLShaderStorageBuffer::OpenGLShaderStorageBuffer(const std::vector<glm::mat4>& storage_data, uint32_t size)
{
    PROFILE_FUNCTION();

    glCreateBuffers(1, &m_RendererID);
    SetData(storage_data, size);
}

OpenGLShaderStorageBuffer::~OpenGLShaderStorageBuffer()
{
    PROFILE_FUNCTION();

    glDeleteBuffers(1, &m_RendererID);
}

void OpenGLShaderStorageBuffer::Bind() const
{
    PROFILE_FUNCTION();

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
}

void OpenGLShaderStorageBuffer::Unbind() const
{
    PROFILE_FUNCTION();

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void OpenGLShaderStorageBuffer::SetData(const std::vector<glm::vec3>& storage_data, uint32_t size)
{
    PROFILE_FUNCTION();

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, GL_STATIC_DRAW);

    GLint buffer_mask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;

    glm::vec3* mapped_buffer = reinterpret_cast<glm::vec3*>(glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, size, buffer_mask));

    std::copy(storage_data.begin(), storage_data.end(), mapped_buffer);

    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

void OpenGLShaderStorageBuffer::SetData(const std::vector<glm::vec4>& storage_data, uint32_t size)
{
    PROFILE_FUNCTION();

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, GL_STATIC_DRAW);

    GLint buffer_mask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;

    glm::vec4* mapped_buffer = reinterpret_cast<glm::vec4*>(glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, size, buffer_mask));

    std::copy(storage_data.begin(), storage_data.end(), mapped_buffer);

    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

void OpenGLShaderStorageBuffer::SetData(const std::vector<glm::uvec4>& storage_data, uint32_t size)
{
    PROFILE_FUNCTION();

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, GL_STATIC_DRAW);

    GLint buffer_mask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;

    glm::uvec4* mapped_buffer = reinterpret_cast<glm::uvec4*>(glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, size, buffer_mask));

    std::copy(storage_data.begin(), storage_data.end(), mapped_buffer);

    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

void OpenGLShaderStorageBuffer::SetData(const std::vector<glm::mat3>& storage_data, uint32_t size)
{
    PROFILE_FUNCTION();

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, GL_STATIC_DRAW);

    GLint buffer_mask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;

    glm::mat3* mapped_buffer = reinterpret_cast<glm::mat3*>(glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, size, buffer_mask));

    std::copy(storage_data.begin(), storage_data.end(), mapped_buffer);

    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

void OpenGLShaderStorageBuffer::SetData(const std::vector<glm::int32>& storage_data, uint32_t size)
{
    PROFILE_FUNCTION();

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, GL_STATIC_DRAW);

    GLint buffer_mask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;

    glm::int32* mapped_buffer = reinterpret_cast<glm::int32*>(glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, size, buffer_mask));

    std::copy(storage_data.begin(), storage_data.end(), mapped_buffer);

    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

void OpenGLShaderStorageBuffer::SetData(const std::vector<glm::mat4>& storage_data, uint32_t size)
{
    PROFILE_FUNCTION();

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, GL_STATIC_DRAW);

    GLint buffer_mask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;

    glm::mat4* mapped_buffer = reinterpret_cast<glm::mat4*>(glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, size, buffer_mask));

    std::copy(storage_data.begin(), storage_data.end(), mapped_buffer);

    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

void OpenGLShaderStorageBuffer::SetPersistentData(const std::vector<glm::vec4>& storage_data, uint32_t size)
{
    PROFILE_FUNCTION();
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);

    GLint buffer_mask = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
    glBufferStorage(GL_SHADER_STORAGE_BUFFER, size, nullptr, buffer_mask);

    persistent_mapped_buffer_ = reinterpret_cast<glm::vec4*>(glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, size, buffer_mask));

    std::copy(storage_data.begin(), storage_data.end(), persistent_mapped_buffer_);
}

void OpenGLShaderStorageBuffer::SetPersistentDataIndex(const glm::vec4& value, size_t index)
{
    reinterpret_cast<glm::vec4*>(persistent_mapped_buffer_)[index] = value;
}

std::vector<glm::vec3> OpenGLShaderStorageBuffer::GetData(uint32_t size)
{
    PROFILE_FUNCTION();

    // TODO(Rok Kos): Improve this function not to copy whole buffer
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
    GLint buffer_mask = GL_MAP_READ_BIT;

    glm::vec3* mapped_buffer = reinterpret_cast<glm::vec3*>(glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, size, buffer_mask));

    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    return std::vector<glm::vec3>(mapped_buffer, mapped_buffer + (size / sizeof(glm::vec3)));
}

/*std::vector<glm::vec4> OpenGLShaderStorageBuffer::GetData(uint32_t size)
{
        // TODO(Rok Kos): Improve this function not to copy whole buffer
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, renderer_id_);
        GLint buffer_mask = GL_MAP_READ_BIT;

        glm::vec4* mapped_buffer = reinterpret_cast<glm::vec4*>(
                glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, size, buffer_mask));

        return std::vector<glm::vec4>(mapped_buffer, mapped_buffer + (size / sizeof(glm::vec4)));
}*/

}
