#pragma once

#include "BlackBirdBox/Renderer/Buffers/FrameBuffer.h"

namespace Platform {

	class OpenGLFrameBuffer : public Core::FrameBuffer
	{
	public:
		OpenGLFrameBuffer(uint32_t width, uint32_t height, bool render_to_camera = false);
		
		virtual ~OpenGLFrameBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		
	private:
		uint32_t renderer_id_;
		uint32_t tex_color_attachment_id_;
		uint32_t tex_depth_stencil_attachment_id_;
		
		uint32_t width_;
		uint32_t height_;
		bool render_to_camera_ = false;
	};

}