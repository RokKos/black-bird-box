﻿#include "bbbpch.h"
#include "OpenGLVertexArray.h"

#include <GL/glew.h>

namespace Platform {

static GLenum ShaderDataTypeToOpenGLBaseType(BlackBirdBox::ShaderDataType type)
{
    switch (type) {
    case BlackBirdBox::ShaderDataType::Float:
        return GL_FLOAT;
    case BlackBirdBox::ShaderDataType::Float2:
        return GL_FLOAT;
    case BlackBirdBox::ShaderDataType::Float3:
        return GL_FLOAT;
    case BlackBirdBox::ShaderDataType::Float4:
        return GL_FLOAT;
    case BlackBirdBox::ShaderDataType::Mat3:
        return GL_FLOAT;
    case BlackBirdBox::ShaderDataType::Mat4:
        return GL_FLOAT;
    case BlackBirdBox::ShaderDataType::Int:
        return GL_INT;
    case BlackBirdBox::ShaderDataType::Int2:
        return GL_INT;
    case BlackBirdBox::ShaderDataType::Int3:
        return GL_INT;
    case BlackBirdBox::ShaderDataType::Int4:
        return GL_INT;
    case BlackBirdBox::ShaderDataType::Bool:
        return GL_BOOL;
    }

    CORE_ASSERT(false, "Unknown ShaderDataType!");
    return 0;
}

OpenGLVertexArray::OpenGLVertexArray()
{
    PROFILE_FUNCTION();

    glCreateVertexArrays(1, &m_RendererID);
}

OpenGLVertexArray::~OpenGLVertexArray()
{
    PROFILE_FUNCTION();

    glDeleteVertexArrays(1, &m_RendererID);
}

void OpenGLVertexArray::Bind() const
{
    PROFILE_FUNCTION();

    glBindVertexArray(m_RendererID);
}

void OpenGLVertexArray::Unbind() const
{
    PROFILE_FUNCTION();

    glBindVertexArray(0);
}

void OpenGLVertexArray::AddVertexBuffer(const BlackBirdBox::Ref<BlackBirdBox::VertexBuffer>& vertexBuffer)
{
    PROFILE_FUNCTION();

    CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

    glBindVertexArray(m_RendererID);
    vertexBuffer->Bind();

    const auto& layout = vertexBuffer->GetLayout();
    for (const auto& element : layout) {
        glEnableVertexAttribArray(m_VertexBufferIndex);
        glVertexAttribPointer(m_VertexBufferIndex, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.Type),
            element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)element.Offset);
        m_VertexBufferIndex++;
    }

    m_VertexBuffers.push_back(vertexBuffer);
}

void OpenGLVertexArray::SetIndexBuffer(const BlackBirdBox::Ref<BlackBirdBox::IndexBuffer>& indexBuffer)
{
    PROFILE_FUNCTION();

    glBindVertexArray(m_RendererID);
    indexBuffer->Bind();

    m_IndexBuffer = indexBuffer;
}

}
