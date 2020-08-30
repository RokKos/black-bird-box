#include "bbbpch.h"

#include "BlackBirdBox/Primitives/GraphVertex.h"
#include "Util.h"

namespace Core {

std::string Util::ReadFile(std::string file_path)
{
    PROFILE_FUNCTION();

    std::stringstream buffer;
    buffer << std::ifstream(file_path).rdbuf();
    return buffer.str();
}

std::vector<unsigned int> Util::GreedyGraphVertexColoring(unsigned int graph_size, std::vector<glm::mat4> neighbours)
{
    PROFILE_FUNCTION();

    std::vector<Ref<GraphVertex>> graph_vertexes;
    graph_vertexes.reserve(graph_size);
    for (unsigned int i = 0; i < graph_size; ++i) {
        graph_vertexes.push_back(CreateRef<GraphVertex>());
    }

    for (unsigned int i = 0; i < graph_size; ++i) {
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

// This is fixed function for connection that I have now
static std::vector<std::vector<glm::vec4>> ClothSeperateEdges(unsigned int graph_size, std::vector<glm::mat4> negibours)
{
    std::vector<glm::vec4> batch_01;
    std::vector<glm::vec4> batch_02;
    for (int y = 0; y < graph_size; ++y) {
        for (int x = 0; x < graph_size; x += 2) {
            batch_01.emplace_back(glm::vec4(x, x + 1, 0, 0));
            batch_02.emplace_back(glm::vec4(x + 1, x + 2, 0, 0));
        }
    }
}

} // namespace Core