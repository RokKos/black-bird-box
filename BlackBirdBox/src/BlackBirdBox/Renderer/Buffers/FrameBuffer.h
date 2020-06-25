#pragma once

#include "BlackBirdBox/Core/Core.h"

namespace Core {
	class FrameBuffer {
	public:
		virtual ~FrameBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Ref<FrameBuffer> Create(uint32_t width, uint32_t height, bool render_to_camera = false);

	};

}