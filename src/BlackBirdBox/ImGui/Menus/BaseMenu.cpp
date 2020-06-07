#include "bbbpch.h"
#include "BaseMenu.h"

#include <imgui.h>

namespace Core {
	
	BaseMenu::BaseMenu(const std::string& menu_name) :
		menu_name_(menu_name)
	{

	}

	void BaseMenu::OnImGuiRender()
	{
		ImGui::Begin(menu_name_.c_str());

		ImGui::End();
	}

}