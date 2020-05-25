#pragma once

#include "ShaderStorageBuffer.h"

namespace Core {

	class ShaderStorageArray
	{
	public:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddShaderStorageBuffer(const Ref<ShaderStorageBuffer>& vertexBuffer) = 0;
		virtual const std::vector<Ref<ShaderStorageBuffer>>& GetShaderStorageBuffers() const = 0;

		static Ref<ShaderStorageArray> Create();
	};

}
