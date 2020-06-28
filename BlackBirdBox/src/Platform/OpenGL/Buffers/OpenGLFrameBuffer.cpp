#include "bbbpch.h"
#include "OpenGLFrameBuffer.h"

#include <GL/glew.h>

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
		texture_color_attachment_ = Core::Texture2D::Create(tex_color_spec);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_color_attachment_->GetRenderID(), 0);

		texture_normal_attachment_ = Core::Texture2D::Create(tex_color_spec);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, texture_normal_attachment_->GetRenderID(), 0);

		GLenum DrawBuffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(2, DrawBuffers);

		Core::Texture2DSpecification tex_depth_stencil_spec = Core::Texture2DSpecification();
		tex_depth_stencil_spec.Width = specification_.width;
		tex_depth_stencil_spec.Height = specification_.height;
		tex_depth_stencil_spec.InternalFormat = Core::ImageFormat::DEPTH24_STENCIL8;
		tex_depth_stencil_spec.DataFormat = Core::ImageFormat::DEPTH_STENCIL;
		tex_depth_stencil_spec.TypeOfData = Core::TypeOfPixelData::UNSIGNED_INT_24_8;
		// TODO(Rok Kos): Check if default parameters from texture like wraping and min/mag filter affect depth/stencil texture
		texture_depth_stencil_attachment_ = Core::Texture2D::Create(tex_depth_stencil_spec);
		
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture_depth_stencil_attachment_->GetRenderID(), 0);

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

}
