#pragma once

#include "BlackBirdBox/Renderer/Buffers/ShaderStorageArray.h"

namespace Platform {

	class OpenGLShaderStorageArray : public Core::ShaderStorageArray
	{
	public:
		OpenGLShaderStorageArray() = default;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual size_t AddShaderStorageBuffer(const Core::Ref<Core::ShaderStorageBuffer>& shader_storage_buffer) override;
		virtual void SetShaderStorageBuffer(int id, const Core::Ref<Core::ShaderStorageBuffer>& shader_storage_buffer) override;
		virtual const std::vector<Core::Ref<Core::ShaderStorageBuffer>>& GetShaderStorageBuffers() const override { return shader_storage_buffers_; }
	private:
		uint32_t m_RendererID;
		std::vector<Core::Ref<Core::ShaderStorageBuffer>> shader_storage_buffers_;
	};

}
