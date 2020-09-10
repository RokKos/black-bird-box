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
std::vector<std::vector<glm::vec4>> Util::ClothSeperateEdges(unsigned int graph_size)
{
    // Horizontal Connections
    std::vector<glm::vec4> batch_01;
    batch_01.reserve(graph_size * (graph_size / 2));
    std::vector<glm::vec4> batch_02;
    for (int y = 0; y < graph_size; ++y) {
        for (int x = 0; x < graph_size - 1; x += 2) {
            batch_01.emplace_back(glm::vec4(y * graph_size + x, y * graph_size + x + 1, 0, 0));
            if (x + 2 < graph_size) {
                batch_02.emplace_back(glm::vec4(y * graph_size + x + 1, y * graph_size + x + 2, 0, 0));
            }
        }
    }

    // Vertical Connections
    std::vector<glm::vec4> batch_03;
    batch_03.reserve(graph_size * (graph_size / 2));
    std::vector<glm::vec4> batch_04;
    for (int y = 0; y < graph_size - 1; y += 2) {
        for (int x = 0; x < graph_size; x++) {
            batch_03.emplace_back(glm::vec4(y * graph_size + x, (y + 1) * graph_size + x, 0, 0));
            if (y + 2 < graph_size) {
                batch_04.emplace_back(glm::vec4((y + 1) * graph_size + x, (y + 2) * graph_size + x, 0, 0));
            }
        }
    }

    // Diagonal Connections
    std::vector<glm::vec4> batch_05;
    for (int y = 0; y < graph_size - 1; y += 2) {
        for (int x = 0; x < graph_size - 1; x += 2) {
            batch_05.emplace_back(glm::vec4(y * graph_size + x, (y + 1) * graph_size + x + 1, 1, 0));
            batch_05.emplace_back(glm::vec4(y * graph_size + x + 1, (y + 1) * graph_size + x, 1, 0));
        }
    }

    std::vector<glm::vec4> batch_06;
    for (int y = 1; y < graph_size - 1; y += 2) {
        for (int x = 0; x < graph_size - 1; x += 2) {
            batch_06.emplace_back(glm::vec4(y * graph_size + x, (y + 1) * graph_size + x + 1, 1, 0));
            batch_06.emplace_back(glm::vec4(y * graph_size + x + 1, (y + 1) * graph_size + x, 1, 0));
        }
    }

    std::vector<glm::vec4> batch_07;
    for (int y = 0; y < graph_size - 1; y += 2) {
        for (int x = 1; x < graph_size - 1; x += 2) {
            batch_07.emplace_back(glm::vec4(y * graph_size + x, (y + 1) * graph_size + x + 1, 1, 0));
            batch_07.emplace_back(glm::vec4(y * graph_size + x + 1, (y + 1) * graph_size + x, 1, 0));
        }
    }

    std::vector<glm::vec4> batch_08;
    for (int y = 1; y < graph_size - 1; y += 2) {
        for (int x = 1; x < graph_size - 1; x += 2) {
            batch_08.emplace_back(glm::vec4(y * graph_size + x, (y + 1) * graph_size + x + 1, 1, 0));
            batch_08.emplace_back(glm::vec4(y * graph_size + x + 1, (y + 1) * graph_size + x, 1, 0));
        }
    }

    return { batch_01, batch_02, batch_03, batch_04, batch_05, batch_06, batch_07, batch_08 };
}

} // namespace Core