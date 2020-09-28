#include "bbbpch.h"
#include "OpenGLFrameBuffer.h"

namespace Platform {

OpenGLFrameBuffer::OpenGLFrameBuffer(const BlackBirdBox::FramebufferSpecification& specification)
    : specification_(specification)
{
    PROFILE_FUNCTION();

    RecreateNewBuffer();
}

OpenGLFrameBuffer::~OpenGLFrameBuffer()
{
    PROFILE_FUNCTION();
    glDeleteFramebuffers(1, &renderer_id_);
}

void OpenGLFrameBuffer::Bind() const
{
    PROFILE_FUNCTION();

    glBindFramebuffer(GL_FRAMEBUFFER, renderer_id_);

    glViewport(0, 0, specification_.width, specification_.height);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLFrameBuffer::Unbind() const
{
    PROFILE_FUNCTION();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
{
    if (width == 0 || height == 0) {
        return;
    }

    specification_.width = width;
    specification_.height = height;

    RecreateNewBuffer();
}

const BlackBirdBox::Ref<BlackBirdBox::Texture2D>& OpenGLFrameBuffer::GetTextureAttachment(BlackBirdBox::FrameBufferAttachments attachment)
{
    return texture_attachments_[attachment];
}

void OpenGLFrameBuffer::RecreateNewBuffer()
{
    if (renderer_id_) {
        glDeleteFramebuffers(1, &renderer_id_);
        texture_attachments_ = std::unordered_map<BlackBirdBox::FrameBufferAttachments, BlackBirdBox::Ref<BlackBirdBox::Texture2D>>();
    }

    glGenFramebuffers(1, &renderer_id_);
    glBindFramebuffer(GL_FRAMEBUFFER, renderer_id_);

    BlackBirdBox::Texture2DSpecification tex_color_spec = BlackBirdBox::Texture2DSpecification();
    tex_color_spec.Width = specification_.width;
    tex_color_spec.Height = specification_.height;
    tex_color_spec.InternalFormat = BlackBirdBox::ImageFormat::RGBA8;
    tex_color_spec.DataFormat = BlackBirdBox::ImageFormat::RGBA;
    tex_color_spec.TextureMagFilter = BlackBirdBox::TextureMagnificationFilter::LINEAR;
    tex_color_spec.TextureMinFilter = BlackBirdBox::TextureMinifyingFilter::LINEAR;

    BlackBirdBox::Texture2DSpecification tex_depth_stencil_spec = BlackBirdBox::Texture2DSpecification();
    tex_depth_stencil_spec.Width = specification_.width;
    tex_depth_stencil_spec.Height = specification_.height;
    tex_depth_stencil_spec.InternalFormat = BlackBirdBox::ImageFormat::DEPTH24_STENCIL8;
    tex_depth_stencil_spec.DataFormat = BlackBirdBox::ImageFormat::DEPTH_STENCIL;
    tex_depth_stencil_spec.TypeOfData = BlackBirdBox::TypeOfPixelData::UNSIGNED_INT_24_8;

    std::vector<GLenum> draw_buffers;
    for (BlackBirdBox::FrameBufferAttachments attachment : specification_.frame_buffer_attachments) {
        GLenum gl_attachment = OpenGLFrameBufferAttachments(attachment);

        if (attachment == BlackBirdBox::FrameBufferAttachments::DEPTH_STENCIL_ATTACHMENT) {
            texture_attachments_[attachment] = BlackBirdBox::Texture2D::Create(tex_depth_stencil_spec);
        } else {
            texture_attachments_[attachment] = BlackBirdBox::Texture2D::Create(tex_color_spec);
            draw_buffers.push_back(gl_attachment);
        }

        glFramebufferTexture2D(GL_FRAMEBUFFER, gl_attachment, GL_TEXTURE_2D, texture_attachments_[attachment]->GetRenderID(), 0);
    }

    glDrawBuffers(draw_buffers.size(), draw_buffers.data());

    CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLenum OpenGLFrameBuffer::OpenGLFrameBufferAttachments(BlackBirdBox::FrameBufferAttachments attachment) const
{
    switch (attachment) {
    case BlackBirdBox::FrameBufferAttachments::COLOR_ATTACHMENT0:
        return GL_COLOR_ATTACHMENT0;
    case BlackBirdBox::FrameBufferAttachments::COLOR_ATTACHMENT1:
        return GL_COLOR_ATTACHMENT1;
    case BlackBirdBox::FrameBufferAttachments::COLOR_ATTACHMENT2:
        return GL_COLOR_ATTACHMENT2;
    case BlackBirdBox::FrameBufferAttachments::COLOR_ATTACHMENT3:
        return GL_COLOR_ATTACHMENT3;
    case BlackBirdBox::FrameBufferAttachments::COLOR_ATTACHMENT4:
        return GL_COLOR_ATTACHMENT4;
    case BlackBirdBox::FrameBufferAttachments::COLOR_ATTACHMENT5:
        return GL_COLOR_ATTACHMENT5;
    case BlackBirdBox::FrameBufferAttachments::COLOR_ATTACHMENT6:
        return GL_COLOR_ATTACHMENT6;
    case BlackBirdBox::FrameBufferAttachments::COLOR_ATTACHMENT7:
        return GL_COLOR_ATTACHMENT7;
    case BlackBirdBox::FrameBufferAttachments::COLOR_ATTACHMENT8:
        return GL_COLOR_ATTACHMENT8;
    case BlackBirdBox::FrameBufferAttachments::COLOR_ATTACHMENT9:
        return GL_COLOR_ATTACHMENT9;
    case BlackBirdBox::FrameBufferAttachments::COLOR_ATTACHMENT10:
        return GL_COLOR_ATTACHMENT10;
    case BlackBirdBox::FrameBufferAttachments::COLOR_ATTACHMENT11:
        return GL_COLOR_ATTACHMENT11;
    case BlackBirdBox::FrameBufferAttachments::COLOR_ATTACHMENT12:
        return GL_COLOR_ATTACHMENT12;
    case BlackBirdBox::FrameBufferAttachments::COLOR_ATTACHMENT13:
        return GL_COLOR_ATTACHMENT13;
    case BlackBirdBox::FrameBufferAttachments::COLOR_ATTACHMENT14:
        return GL_COLOR_ATTACHMENT14;
    case BlackBirdBox::FrameBufferAttachments::COLOR_ATTACHMENT15:
        return GL_COLOR_ATTACHMENT15;
    case BlackBirdBox::FrameBufferAttachments::DEPTH_ATTACHMENT:
        return GL_DEPTH_ATTACHMENT;
    case BlackBirdBox::FrameBufferAttachments::STENCIL_ATTACHMENT:
        return GL_STENCIL_ATTACHMENT;
    case BlackBirdBox::FrameBufferAttachments::DEPTH_STENCIL_ATTACHMENT:
        return GL_DEPTH_STENCIL_ATTACHMENT;
    }

    CORE_ASSERT(false, "FrameBufferAttachments unknown");
    return 0;
}

}
