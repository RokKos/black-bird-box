#pragma once

#include "BlackBirdBox/Core/Core.h"

namespace Core {

	class GraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

		virtual const std::array<int, 3>& GetWorkGroupSize() const = 0;
		virtual const std::array<int, 3>& GetLocalGroupSize() const = 0;
		virtual int GetMaxComputeShaderInvocations() const = 0;

		static Scope<GraphicsContext> Create(void* window);
	};

}