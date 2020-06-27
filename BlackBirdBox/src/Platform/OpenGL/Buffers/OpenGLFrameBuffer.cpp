#include "bbbpch.h"
#include "OpenGLFrameBuffer.h"

#include <GL/glew.h>

namespace Platform {

	OpenGLFrameBuffer::OpenGLFrameBuffer(uint32_t width, uint32_t height, bool render_to_camera) :
		width_(width), height_(height), render_to_camera_(render_to_camera)
	{
		PROFILE_FUNCTION();

		glGenFramebuffers(1, &renderer_id_);
		glBindFramebuffer(GL_FRAMEBUFFER, renderer_id_);

		Core::Texture2DSpecification tex_color_spec = Core::Texture2DSpecification();
		tex_color_spec.Width = width;
		tex_color_spec.Height = height;
		tex_color_spec.InternalFormat = Core::ImageFormat::RGBA8;
		tex_color_spec.DataFormat = Core::ImageFormat::RGBA;
		tex_color_spec.TextureMagFilter = Core::TextureMagnificationFilter::LINEAR;
		tex_color_spec.TextureMinFilter = Core::TextureMinifyingFilter::LINEAR;
		texture_color_attachment_ = Core::Texture2D::Create(tex_color_spec);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_color_attachment_->GetRenderID(), 0);

		/*Core::Texture2DSpecification tex_depth_stencil_spec = Core::Texture2DSpecification();
		tex_depth_stencil_spec.Width = width;
		tex_depth_stencil_spec.Height = height;
		tex_depth_stencil_spec.InternalFormat = Core::ImageFormat::DEPTH24_STENCIL8;
		tex_depth_stencil_spec.DataFormat = Core::ImageFormat::DEPTH_STENCIL;
		tex_depth_stencil_spec.TypeOfData = Core::TypeOfPixelData::UNSIGNED_INT_24_8;
		// TODO(Rok Kos): Check if default parameters from texture like wraping and min/mag filter affect depth/stencil texture
		texture_depth_stencil_attachment_ = Core::Texture2D::Create(tex_depth_stencil_spec);
		*/

		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
		glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0,GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
		//glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

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
		glViewport(0, 0, width_, height_);
	}

	void OpenGLFrameBuffer::Unbind() const
	{
		PROFILE_FUNCTION();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::BindTextureColorAttachment() const
	{
		glBindTexture(GL_TEXTURE_2D, texture_color_attachment_->GetRenderID());
	}

}
