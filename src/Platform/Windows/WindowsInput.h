#pragma once

#include "BlackBirdBox/Core/Input.h"

namespace Platform{

	class WindowsInput : public Core::Input
	{
	protected:
		virtual bool IsKeyPressedImpl(Core::KeyCode key) override;

		virtual bool IsMouseButtonPressedImpl(Core::MouseCode button) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};

}