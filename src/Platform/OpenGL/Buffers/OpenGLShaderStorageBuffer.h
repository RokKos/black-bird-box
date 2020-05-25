#pragma once

#include "BlackBirdBox/Renderer/Buffers/ShaderStorageBuffer.h"

namespace Platform {

	class OpenGLShaderStorageBuffer : public Core::ShaderStorageBuffer
	{
	public:
		OpenGLShaderStorageBuffer(uint32_t size);
		OpenGLShaderStorageBuffer(const std::vector<glm::vec3>& storage_data, uint32_t size);
		OpenGLShaderStorageBuffer(const std::vector<glm::vec4>& storage_data, uint32_t size);
		virtual ~OpenGLShaderStorageBuffer();

		virtual uint32_t GetRendererID() const override { return m_RendererID; };
		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(const std::vector<glm::vec3>& storage_data, uint32_t size) override;
		virtual void SetData(const std::vector<glm::vec4>& storage_data, uint32_t size) override;
		
		virtual std::vector<glm::vec3> GetData(uint32_t size) override;
		//virtual std::vector<glm::vec4> GetData(uint32_t size) override;


	private:
		uint32_t m_RendererID;
	};

}