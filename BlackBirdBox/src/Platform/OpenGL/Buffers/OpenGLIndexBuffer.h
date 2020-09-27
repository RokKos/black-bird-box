﻿#pragma once

#include "BlackBirdBox/Renderer/Buffers/IndexBuffer.h"

namespace Platform {

class OpenGLIndexBuffer : public BlackBirdBox::IndexBuffer {
public:
    OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
    virtual ~OpenGLIndexBuffer();

    virtual void Bind() const;
    virtual void Unbind() const;

    virtual uint32_t GetCount() const { return m_Count; }

private:
    uint32_t m_RendererID;
    uint32_t m_Count;
};
}
