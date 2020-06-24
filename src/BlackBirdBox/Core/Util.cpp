#include "bbbpch.h"
#include "Util.h"

#include "BlackBirdBox/Primitives/GraphVertex.h"

namespace Core {
	
	std::string Util::ReadFile(std::string file_path)
	{
		std::stringstream buffer;
		buffer << std::ifstream(file_path).rdbuf();
		return buffer.str();
	}

	std::vector<unsigned int> Util::GreedyGraphColoring(unsigned int graph_size, std::vector<glm::mat4> neighbours)
	{
		std::vector<Ref<GraphVertex>> graph_vertexes;
		graph_vertexes.reserve(graph_size);
		for (unsigned int i = 0; i < graph_size; ++i) {
			graph_vertexes.push_back(CreateRef<GraphVertex>());
		}
		

		for (unsigned int i = 0; i < graph_size; ++i){
			std::vector<Ref<GraphVertex>> graph_neighbours;
			graph_neighbours.reserve(8);
			for (int x = 0; x < 3; ++x) {
				for (int y = 0; y < 3; ++y) {
					int neighbour_index = static_cast<int>(neighbours[i][x][y]);
					if (neighbour_index < 0) {
						continue;
					}
					graph_neighbours.push_back(graph_vertexes[neighbour_index]);
				}
			}
			graph_vertexes[i]->SetConnections(graph_neighbours);
		}

		for (unsigned int i = 0; i < graph_size; ++i) {
			graph_vertexes[i]->SelectColor();
		}

		std::vector<unsigned int> graph_coloring(graph_size, 0);
		for (unsigned int i = 0; i < graph_size; ++i) {
			graph_coloring[i] = graph_vertexes[i]->GetColor();
		}

		return graph_coloring;
	}

}