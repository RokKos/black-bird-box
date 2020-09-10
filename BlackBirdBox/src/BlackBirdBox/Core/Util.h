#pragma once

#include <glm/glm.hpp>

namespace Core {

class Util {
public:
    static std::string ReadFile(std::string file_path);
    static std::vector<unsigned int> GreedyGraphVertexColoring(unsigned int graph_size, std::vector<glm::mat4> negibours);
    static std::vector<std::vector<glm::vec4>> ClothSeperateEdges(unsigned int cloth_dimension);
};

}
