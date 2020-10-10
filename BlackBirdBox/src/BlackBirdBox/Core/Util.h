﻿#pragma once

#include <glm/glm.hpp>

namespace BlackBirdBox {

class Util {
public:
    static std::string ReadFile(std::string file_path);
    static std::vector<uint32_t> GreedyGraphVertexColoring(uint32_t graph_size, std::vector<glm::mat4> neighbours);
    static std::vector<std::vector<glm::vec4>> ClothSeperateEdges(uint32_t cloth_dimension);
};

}
