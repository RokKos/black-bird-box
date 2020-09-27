#pragma once

#include "BlackBirdBox/Renderer/Buffers/VertexArray.h"

namespace Platform {

class OpenGLVertexArray : public BlackBirdBox::VertexArray {
public:
    OpenGLVertexArray();
    virtual ~OpenGLVertexArray();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void AddVertexBuffer(const BlackBirdBox::Ref<BlackBirdBox::VertexBuffer>& vertexBuffer) override;
    virtual void SetIndexBuffer(const BlackBirdBox::Ref<BlackBirdBox::IndexBuffer>& indexBuffer) override;

    virtual const std::vector<BlackBirdBox::Ref<BlackBirdBox::VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }
    virtual const BlackBirdBox::Ref<BlackBirdBox::IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }

private:
    uint32_t m_RendererID;
    uint32_t m_VertexBufferIndex = 0;
    std::vector<BlackBirdBox::Ref<BlackBirdBox::VertexBuffer>> m_VertexBuffers;
    BlackBirdBox::Ref<BlackBirdBox::IndexBuffer> m_IndexBuffer;
};

}
