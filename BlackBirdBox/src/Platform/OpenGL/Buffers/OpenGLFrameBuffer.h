#pragma once

#include "BlackBirdBox/Renderer/Buffers/FrameBuffer.h"

namespace Platform {

	class OpenGLFrameBuffer : public Core::FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const Core::FramebufferSpecification& specification);
		
		virtual ~OpenGLFrameBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const Core::Ref<Core::Texture2D>& GetTextureColorAttachment() const { return texture_color_attachment_;	};
		virtual const Core::Ref<Core::Texture2D>& GetTextureNormalAttachment() const { return texture_normal_attachment_; };
		virtual const Core::Ref<Core::Texture2D>& GetTextureDepthStencilAttachment() const { return texture_depth_stencil_attachment_; };

		
	private:
		uint32_t renderer_id_;
		Core::Ref<Core::Texture2D> texture_color_attachment_;
		Core::Ref<Core::Texture2D> texture_normal_attachment_;
		Core::Ref<Core::Texture2D> texture_depth_stencil_attachment_;


		Core::FramebufferSpecification specification_;
	};

}