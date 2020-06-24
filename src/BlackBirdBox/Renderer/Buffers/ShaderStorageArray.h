#pragma once

#include "ShaderStorageBuffer.h"

namespace Core {

	class ShaderStorageArray
	{
	public:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual size_t AddShaderStorageBuffer(const Ref<ShaderStorageBuffer>& vertexBuffer) = 0;
		virtual void SetShaderStorageBuffer(int id, const Core::Ref<Core::ShaderStorageBuffer>& shader_storage_buffer) = 0;
		virtual const std::vector<Ref<ShaderStorageBuffer>>& GetShaderStorageBuffers() const = 0;

		static Ref<ShaderStorageArray> Create();
	};

}
