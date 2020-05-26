#pragma once

#include "BlackBirdBox.h"

#include <string>

namespace EOL {

	class GeneralSettings {
	public:
		GeneralSettings() : 
			online(false), 
			exportObjs(false), 
			RESOURCE_DIR(""), 
			OUTPUT_DIR("") 
		{
		};

		virtual ~GeneralSettings() {};

		bool online;
		bool exportObjs;
		bool exportTimings;
		std::string RESOURCE_DIR;
		std::string OUTPUT_DIR;

		void printGenSet() {
			LOG_INFO("General settings");
			LOG_INFO("	online: {0}", printGenBool(online));
			LOG_INFO("	exportObjs: {0}", printGenBool(exportObjs));
			LOG_INFO("	exportTimings: {0}", printGenBool(exportTimings));
			LOG_INFO("	RESOURCE_DIR: {0}", RESOURCE_DIR);
			LOG_INFO("	OUTPUT_DIR: {0}", OUTPUT_DIR);
		}

	private:

		std::string printGenBool(bool b) {
			return b ? "True" : "False";
		}

	};

}