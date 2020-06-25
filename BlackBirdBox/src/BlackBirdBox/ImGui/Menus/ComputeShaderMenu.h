#pragma once

#include "BaseMenu.h"
#include "BlackBirdBox/Primitives/ComputeShaderSimulationConfiguration.h"

namespace Core {

	class ComputeShaderMenu : public BaseMenu
	{
	public:
		ComputeShaderMenu(const std::string& menu_name, ComputeShaderSimulationConfiguration& compute_shader_simulation_configuration);

		virtual void OnImGuiRender() override;

	private:
		ComputeShaderSimulationConfiguration& compute_shader_simulation_configuration_;
	};
}