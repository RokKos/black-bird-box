#include "bbbpch.h"
#include "Input.h"

#ifdef PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsInput.h"
#endif

namespace Core {

	Scope<Input> Input::s_Instance = Input::Create();

	Scope<Input> Input::Create()
	{
#ifdef PLATFORM_WINDOWS
		return CreateScope<Platform::WindowsInput>();
#else
		CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}
}