#pragma once

#include "BlackBirdBox/Renderer/Buffers/FrameBuffer.h"

#include <GL/glew.h>

namespace Platform {

	class OpenGLFrameBuffer : public Core::FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const Core::FramebufferSpecification& specification);
		
		virtual ~OpenGLFrameBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const Core::Ref<Core::Texture2D>& GetTextureAttachment(Core::FrameBufferAttachments attachment) override;
		virtual const Core::FramebufferSpecification& GetFrameBufferSpecification() const { return specification_; };

	private:
		GLenum OpenGLFrameBufferAttachments(Core::FrameBufferAttachments attachment) const;
	private:
		uint32_t renderer_id_;
		std::unordered_map<Core::FrameBufferAttachments, Core::Ref<Core::Texture2D>> texture_attachments_;

		Core::FramebufferSpecification specification_;
	};

}