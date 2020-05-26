#pragma once

#include "EOLLayer.h"
#include "BlackBirdBox.h"
#include "BlackBirdBox/Core/EntryPoint.h"

namespace EOL {

	class EOLApplication : public Core::Application
	{
	public:
		EOLApplication();

		~EOLApplication()
		{
		}
		

		const std::string& GetGeneralSettingFilePath() const {
			return general_settings_file_path_;
		}

		const std::string& GetSimulationSettingFilePath() const {
			return simulation_settings_file_path_;
		}

	private:
		std::string general_settings_file_path_ = "assets/";
		std::string simulation_settings_file_path_;
	};



}