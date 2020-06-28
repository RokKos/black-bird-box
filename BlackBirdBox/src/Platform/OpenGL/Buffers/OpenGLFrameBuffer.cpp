#include "bbbpch.h"
#include "OpenGLFrameBuffer.h"

namespace Platform {

	OpenGLFrameBuffer::OpenGLFrameBuffer(const Core::FramebufferSpecification& specification) :
		specification_(specification)
	{
		PROFILE_FUNCTION();

		glGenFramebuffers(1, &renderer_id_);
		glBindFramebuffer(GL_FRAMEBUFFER, renderer_id_);

		Core::Texture2DSpecification tex_color_spec = Core::Texture2DSpecification();
		tex_color_spec.Width = specification_.width;
		tex_color_spec.Height = specification_.height;
		tex_color_spec.InternalFormat = Core::ImageFormat::RGBA8;
		tex_color_spec.DataFormat = Core::ImageFormat::RGBA;
		tex_color_spec.TextureMagFilter = Core::TextureMagnificationFilter::LINEAR;
		tex_color_spec.TextureMinFilter = Core::TextureMinifyingFilter::LINEAR;
		
		Core::Texture2DSpecification tex_depth_stencil_spec = Core::Texture2DSpecification();
		tex_depth_stencil_spec.Width = specification_.width;
		tex_depth_stencil_spec.Height = specification_.height;
		tex_depth_stencil_spec.InternalFormat = Core::ImageFormat::DEPTH24_STENCIL8;
		tex_depth_stencil_spec.DataFormat = Core::ImageFormat::DEPTH_STENCIL;
		tex_depth_stencil_spec.TypeOfData = Core::TypeOfPixelData::UNSIGNED_INT_24_8;

		std::vector<GLenum> draw_buffers;
		for (Core::FrameBufferAttachments attachment : specification_.frame_buffer_attachments) {
			GLenum gl_attachment = OpenGLFrameBufferAttachments(attachment);
			
			if (attachment == Core::FrameBufferAttachments::DEPTH_STENCIL_ATTACHMENT) {
				texture_attachments_[attachment] = Core::Texture2D::Create(tex_depth_stencil_spec);
			}
			else {
				texture_attachments_[attachment] = Core::Texture2D::Create(tex_color_spec);
				draw_buffers.push_back(gl_attachment);
			}
			
			glFramebufferTexture2D(GL_FRAMEBUFFER, gl_attachment, GL_TEXTURE_2D, texture_attachments_[attachment]->GetRenderID(), 0);
			
		}

		glDrawBuffers(draw_buffers.size(), draw_buffers.data());

		CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

	const Core::Ref<Core::Texture2D>& OpenGLFrameBuffer::GetTextureAttachment(Core::FrameBufferAttachments attachment)
	{
		return texture_attachments_[attachment];
	}

	GLenum OpenGLFrameBuffer::OpenGLFrameBufferAttachments(Core::FrameBufferAttachments attachment) const
	{
		switch(attachment) {
			case Core::FrameBufferAttachments::COLOR_ATTACHMENT0: return GL_COLOR_ATTACHMENT0;
			case Core::FrameBufferAttachments::COLOR_ATTACHMENT1: return GL_COLOR_ATTACHMENT1;
			case Core::FrameBufferAttachments::COLOR_ATTACHMENT2: return GL_COLOR_ATTACHMENT2;
			case Core::FrameBufferAttachments::COLOR_ATTACHMENT3: return GL_COLOR_ATTACHMENT3;
			case Core::FrameBufferAttachments::COLOR_ATTACHMENT4: return GL_COLOR_ATTACHMENT4;
			case Core::FrameBufferAttachments::COLOR_ATTACHMENT5: return GL_COLOR_ATTACHMENT5;
			case Core::FrameBufferAttachments::COLOR_ATTACHMENT6: return GL_COLOR_ATTACHMENT6;
			case Core::FrameBufferAttachments::COLOR_ATTACHMENT7: return GL_COLOR_ATTACHMENT7;
			case Core::FrameBufferAttachments::COLOR_ATTACHMENT8: return GL_COLOR_ATTACHMENT8;
			case Core::FrameBufferAttachments::COLOR_ATTACHMENT9: return GL_COLOR_ATTACHMENT9;
			case Core::FrameBufferAttachments::COLOR_ATTACHMENT10: return GL_COLOR_ATTACHMENT10;
			case Core::FrameBufferAttachments::COLOR_ATTACHMENT11: return GL_COLOR_ATTACHMENT11;
			case Core::FrameBufferAttachments::COLOR_ATTACHMENT12: return GL_COLOR_ATTACHMENT12;
			case Core::FrameBufferAttachments::COLOR_ATTACHMENT13: return GL_COLOR_ATTACHMENT13;
			case Core::FrameBufferAttachments::COLOR_ATTACHMENT14: return GL_COLOR_ATTACHMENT14;
			case Core::FrameBufferAttachments::COLOR_ATTACHMENT15: return GL_COLOR_ATTACHMENT15;
			case Core::FrameBufferAttachments::DEPTH_ATTACHMENT: return GL_DEPTH_ATTACHMENT;
			case Core::FrameBufferAttachments::STENCIL_ATTACHMENT: return GL_STENCIL_ATTACHMENT;
			case Core::FrameBufferAttachments::DEPTH_STENCIL_ATTACHMENT: return GL_DEPTH_STENCIL_ATTACHMENT;
		}

		CORE_ASSERT(false, "FrameBufferAttachments unknown");
		return 0;
	}

}
