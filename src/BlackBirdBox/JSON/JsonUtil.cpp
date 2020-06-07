#include "bbbpch.h"
#include "JsonUtil.h"

#include "BlackBirdBox/Core/Core.h"
#include "BlackBirdBox/Core/Util.h"

namespace Core {

	rapidjson::Document JsonUtil::ReadJson(std::string file_path)
	{
		std::string file_content = Util::ReadFile(file_path);
		rapidjson::Document document;
		document.Parse(file_content.c_str());

		CORE_ASSERT(document.IsObject(), "JSON is not valid. Root of JSON needs to be object");
		return document;
	}

}