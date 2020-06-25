#include "bbbpch.h"
#include "BaseMenu.h"

#include <imgui.h>

namespace Core {
	
	BaseMenu::BaseMenu(const std::string& menu_name) :
		menu_name_(menu_name)
	{
		PROFILE_FUNCTION();
	}

	void BaseMenu::OnImGuiRender()
	{
		PROFILE_FUNCTION();

		ImGui::Begin(menu_name_.c_str());

		ImGui::End();
	}

}