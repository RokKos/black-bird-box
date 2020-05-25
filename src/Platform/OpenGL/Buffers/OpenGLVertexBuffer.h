#pragma once

#include "BlackBirdBox/Renderer/Buffers/VertexBuffer.h"

namespace Platform {

	class OpenGLVertexBuffer : public Core::VertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(Core::Vertex* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(const void* data, uint32_t size) override;

		virtual const Core::BufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const Core::BufferLayout& layout) override { m_Layout = layout; }
	private:
		uint32_t m_RendererID;
		Core::BufferLayout m_Layout;
	};

}