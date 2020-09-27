#pragma once

#include "rapidjson/document.h"

namespace BlackBirdBox {

class JsonUtil {
public:
    static rapidjson::Document ReadJson(std::string file_path);
};

}
