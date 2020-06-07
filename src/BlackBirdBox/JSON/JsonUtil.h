#pragma once

#include "document.h"

namespace Core {

	class JsonUtil
	{
	public:
		static rapidjson::Document ReadJson(std::string file_path);
	};

}