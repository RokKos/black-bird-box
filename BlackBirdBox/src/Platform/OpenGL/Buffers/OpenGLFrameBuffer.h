#pragma once

#include "BlackBirdBox/Renderer/Buffers/FrameBuffer.h"

#include <GL/glew.h>

namespace Platform {

class OpenGLFrameBuffer : public BlackBirdBox::FrameBuffer {
public:
    OpenGLFrameBuffer(const BlackBirdBox::FramebufferSpecification& specification);

    virtual ~OpenGLFrameBuffer();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void Resize(uint32_t width, uint32_t height) override;
    virtual const BlackBirdBox::Ref<BlackBirdBox::Texture2D>& GetTextureAttachment(BlackBirdBox::FrameBufferAttachments attachment) override;
    virtual const BlackBirdBox::FramebufferSpecification& GetFrameBufferSpecification() const override { return specification_; };

private:
    void RecreateNewBuffer();
    GLenum OpenGLFrameBufferAttachments(BlackBirdBox::FrameBufferAttachments attachment) const;

private:
    uint32_t renderer_id_;
    std::unordered_map<BlackBirdBox::FrameBufferAttachments, BlackBirdBox::Ref<BlackBirdBox::Texture2D>> texture_attachments_;

    BlackBirdBox::FramebufferSpecification specification_;
};

}
