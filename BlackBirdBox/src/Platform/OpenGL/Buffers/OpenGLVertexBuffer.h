#pragma once

#include "BlackBirdBox/Renderer/Buffers/VertexBuffer.h"

namespace Platform {

	class OpenGLVertexBuffer : public Core::VertexBuffer
	{
	public:
		OpenGLVertexBuffer(Core::Vertex* vertices, uint32_t size);
		OpenGLVertexBuffer(glm::vec3* vertices, uint32_t size);
		OpenGLVertexBuffer(float* vertices, uint32_t size);

		OpenGLVertexBuffer(uint32_t render_id);

		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(const void* data, uint32_t size) override;

		virtual const Core::BufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const Core::BufferLayout& layout) override { m_Layout = layout; }
	private:
		uint32_t renderer_id_;
		Core::BufferLayout m_Layout;
	};

}