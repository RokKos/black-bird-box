﻿#include "bbbpch.h"
#include "OpenGLVertexBuffer.h"

#include <GL/glew.h>

namespace Platform {

OpenGLVertexBuffer::OpenGLVertexBuffer(BlackBirdBox::Vertex* vertices, uint32_t size)
{
    PROFILE_FUNCTION();

    glCreateBuffers(1, &renderer_id_);
    glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(glm::vec3* vertices, uint32_t size)
{
    PROFILE_FUNCTION();

    glCreateBuffers(1, &renderer_id_);
    glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
{
    PROFILE_FUNCTION();

    glCreateBuffers(1, &renderer_id_);
    glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t render_id)
    : renderer_id_(render_id)
{
    PROFILE_FUNCTION();

    glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
    PROFILE_FUNCTION();

    glDeleteBuffers(1, &renderer_id_);
}

void OpenGLVertexBuffer::Bind() const
{
    PROFILE_FUNCTION();

    glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
}

void OpenGLVertexBuffer::Unbind() const
{
    PROFILE_FUNCTION();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
{
    PROFILE_FUNCTION();

    glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}
}
