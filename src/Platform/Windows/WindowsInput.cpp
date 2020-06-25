#include "bbbpch.h"
#include "WindowsInput.h"

#include "BlackBirdBox/Core/Application.h"
#include <GLFW/glfw3.h>

namespace Platform {

	bool WindowsInput::IsKeyPressedImpl(Core::KeyCode key)
	{
		PROFILE_FUNCTION();

		auto window = static_cast<GLFWwindow*>(Core::Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, static_cast<int32_t>(key));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(Core::MouseCode button)
	{
		PROFILE_FUNCTION();

		auto window = static_cast<GLFWwindow*>(Core::Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));
		return state == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		PROFILE_FUNCTION();

		auto window = static_cast<GLFWwindow*>(Core::Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}

	float WindowsInput::GetMouseXImpl()
	{
		PROFILE_FUNCTION();

		auto [x, y] = GetMousePositionImpl();
		return x;
	}

	float WindowsInput::GetMouseYImpl()
	{
		PROFILE_FUNCTION();

		auto [x, y] = GetMousePositionImpl();
		return y;
	}

}