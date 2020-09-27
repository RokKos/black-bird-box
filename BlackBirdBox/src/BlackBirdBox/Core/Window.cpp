#include "bbbpch.h"
#include "Window.h"

#ifdef PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif

namespace BlackBirdBox {

Scope<Window> Window::Create(const WindowProps& props)
{
#ifdef PLATFORM_WINDOWS
    return CreateScope<Platform::WindowsWindow>(props);
#else
    CORE_ASSERT(false, "Unknown platform!");
    return nullptr;
#endif
}

}
