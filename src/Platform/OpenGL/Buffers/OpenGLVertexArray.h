#pragma once

#include "BlackBirdBox/Renderer/Buffers/VertexArray.h"

namespace Platform {

	class OpenGLVertexArray : public Core::VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Core::Ref<Core::VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Core::Ref<Core::IndexBuffer>& indexBuffer) override;

		virtual const std::vector<Core::Ref<Core::VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }
		virtual const Core::Ref<Core::IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }
	private:
		uint32_t m_RendererID;
		uint32_t m_VertexBufferIndex = 0;
		std::vector<Core::Ref<Core::VertexBuffer>> m_VertexBuffers;
		Core::Ref<Core::IndexBuffer> m_IndexBuffer;
	};

}
