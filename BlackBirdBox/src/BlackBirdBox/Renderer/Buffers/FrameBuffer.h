#pragma once

#include "BlackBirdBox/Core/Core.h"
#include "BlackBirdBox/Renderer/Texture/Texture2D.h"

namespace BlackBirdBox {

enum class FrameBufferAttachments {
    COLOR_ATTACHMENT0,
    COLOR_ATTACHMENT1,
    COLOR_ATTACHMENT2,
    COLOR_ATTACHMENT3,
    COLOR_ATTACHMENT4,
    COLOR_ATTACHMENT5,
    COLOR_ATTACHMENT6,
    COLOR_ATTACHMENT7,
    COLOR_ATTACHMENT8,
    COLOR_ATTACHMENT9,
    COLOR_ATTACHMENT10,
    COLOR_ATTACHMENT11,
    COLOR_ATTACHMENT12,
    COLOR_ATTACHMENT13,
    COLOR_ATTACHMENT14,
    COLOR_ATTACHMENT15,
    DEPTH_ATTACHMENT,
    STENCIL_ATTACHMENT,
    DEPTH_STENCIL_ATTACHMENT
};

struct FramebufferSpecification {
    uint32_t width, height;
    std::vector<FrameBufferAttachments> frame_buffer_attachments{ FrameBufferAttachments::COLOR_ATTACHMENT0,
        FrameBufferAttachments::DEPTH_STENCIL_ATTACHMENT };
    bool render_to_camera = false;
};

class FrameBuffer {
public:
    virtual ~FrameBuffer() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual const Ref<Texture2D>& GetTextureAttachment(FrameBufferAttachments attachment) = 0;
    virtual const FramebufferSpecification& GetFrameBufferSpecification() const = 0;

    static Ref<FrameBuffer> Create(const FramebufferSpecification& specification);
};

}
