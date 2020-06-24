#pragma once

#include <glm/glm.hpp>

namespace Core {

	class Util
	{
	public:
		static std::string ReadFile(std::string file_path);
		static std::vector<unsigned int> GreedyGraphColoring(unsigned int graph_size, std::vector<glm::mat4> negibours);
	};

}
