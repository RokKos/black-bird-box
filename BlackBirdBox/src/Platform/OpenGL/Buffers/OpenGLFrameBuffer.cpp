#include "bbbpch.h"
#include "OpenGLFrameBuffer.h"

#include <GL/glew.h>

namespace Platform {

	OpenGLFrameBuffer::OpenGLFrameBuffer(uint32_t width, uint32_t height, bool render_to_camera) :
		width_(width), height_(height), render_to_camera_(render_to_camera)
	{
		glGenFramebuffers(1, &renderer_id_);
		glBindFramebuffer(GL_FRAMEBUFFER, renderer_id_);

		glGenTextures(1, &tex_color_attachment_id_);
		glBindTexture(GL_TEXTURE_2D, tex_color_attachment_id_);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_color_attachment_id_, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &tex_depth_stencil_attachment_id_);
		glBindTexture(GL_TEXTURE_2D, tex_depth_stencil_attachment_id_);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, width_, height_);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, tex_depth_stencil_attachment_id_, 0);

		CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &renderer_id_);
		glDeleteTextures(1, &tex_color_attachment_id_);
		glDeleteTextures(1, &tex_depth_stencil_attachment_id_);
	}

	void OpenGLFrameBuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, renderer_id_);
		glViewport(0, 0, width_, height_);
	}

	void OpenGLFrameBuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}
