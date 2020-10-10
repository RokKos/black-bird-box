#include "bbbpch.h"

#include "BlackBirdBox/Primitives/GraphVertex.h"
#include "Util.h"

namespace BlackBirdBox {

std::string Util::ReadFile(std::string file_path)
{
    PROFILE_FUNCTION();

    std::stringstream buffer;
    buffer << std::ifstream(file_path).rdbuf();
    return buffer.str();
}

std::vector<uint32_t> Util::GreedyGraphVertexColoring(uint32_t graph_size, std::vector<glm::mat4> neighbours)
{
    PROFILE_FUNCTION();

    std::vector<Ref<GraphVertex>> graph_vertexes;
    graph_vertexes.reserve(graph_size);
    for (uint32_t i = 0; i < graph_size; ++i) {
        graph_vertexes.push_back(CreateRef<GraphVertex>());
    }

    for (uint32_t i = 0; i < graph_size; ++i) {
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

    for (uint32_t i = 0; i < graph_size; ++i) {
        graph_vertexes[i]->SelectColor();
    }

    std::vector<uint32_t> graph_coloring(graph_size, 0);
    for (uint32_t i = 0; i < graph_size; ++i) {
        graph_coloring[i] = graph_vertexes[i]->GetColor();
    }

    return graph_coloring;
}

// This is fixed function for connection that I have now
std::vector<std::vector<glm::uvec4>> Util::ClothSeperateEdges(uint32_t graph_dimension)
{
    PROFILE_FUNCTION();
    // Horizontal Connections
    std::vector<glm::uvec4> batch_01;
    batch_01.reserve(graph_dimension * (graph_dimension / 2));
    std::vector<glm::uvec4> batch_02;
    for (int y = 0; y < graph_dimension; ++y) {
        const int start_of_row = y * graph_dimension;
        for (int x = 0; x < graph_dimension - 1; x += 2) {
            batch_01.emplace_back(glm::uvec4(start_of_row + x, start_of_row + x + 1, 0, 0));
            if (x + 2 < graph_dimension) {
                batch_02.emplace_back(glm::uvec4(start_of_row + x + 1, start_of_row + x + 2, 0, 0));
            }
        }
    }

    // Vertical Connections
    std::vector<glm::uvec4> batch_03;
    batch_03.reserve(graph_dimension * (graph_dimension / 2));
    std::vector<glm::uvec4> batch_04;
    for (int y = 0; y < graph_dimension - 1; y += 2) {
        for (int x = 0; x < graph_dimension; x++) {
            batch_03.emplace_back(glm::uvec4(y * graph_dimension + x, (y + 1) * graph_dimension + x, 0, 0));
            if (y + 2 < graph_dimension) {
                batch_04.emplace_back(glm::uvec4((y + 1) * graph_dimension + x, (y + 2) * graph_dimension + x, 0, 0));
            }
        }
    }

    // Diagonal Connections
    std::vector<glm::uvec4> batch_05;
    for (int y = 0; y < graph_dimension - 1; y += 2) {
        const int start_of_row = y * graph_dimension;
        for (int x = 0; x < graph_dimension - 1; x += 2) {
            batch_05.emplace_back(glm::uvec4(start_of_row + x, (y + 1) * graph_dimension + x + 1, 1, 0));
            batch_05.emplace_back(glm::uvec4(start_of_row + x + 1, (y + 1) * graph_dimension + x, 1, 0));
        }
    }

    std::vector<glm::uvec4> batch_06;
    for (int y = 1; y < graph_dimension - 1; y += 2) {
        for (int x = 0; x < graph_dimension - 1; x += 2) {
            batch_06.emplace_back(glm::uvec4(y * graph_dimension + x, (y + 1) * graph_dimension + x + 1, 1, 0));
            batch_06.emplace_back(glm::uvec4(y * graph_dimension + x + 1, (y + 1) * graph_dimension + x, 1, 0));
        }
    }

    std::vector<glm::uvec4> batch_07;
    for (int y = 0; y < graph_dimension - 1; y += 2) {
        for (int x = 1; x < graph_dimension - 1; x += 2) {
            batch_07.emplace_back(glm::uvec4(y * graph_dimension + x, (y + 1) * graph_dimension + x + 1, 1, 0));
            batch_07.emplace_back(glm::uvec4(y * graph_dimension + x + 1, (y + 1) * graph_dimension + x, 1, 0));
        }
    }

    std::vector<glm::uvec4> batch_08;
    for (int y = 1; y < graph_dimension - 1; y += 2) {
        for (int x = 1; x < graph_dimension - 1; x += 2) {
            batch_08.emplace_back(glm::uvec4(y * graph_dimension + x, (y + 1) * graph_dimension + x + 1, 1, 0));
            batch_08.emplace_back(glm::uvec4(y * graph_dimension + x + 1, (y + 1) * graph_dimension + x, 1, 0));
        }
    }

    // Skip One connection (Bending connections)
    std::vector<glm::uvec4> batch_09;
    std::vector<glm::uvec4> batch_10;
    for (int y = 0; y < graph_dimension; ++y) {
        const int start_of_row = y * graph_dimension;
        for (int x = 0; x < graph_dimension; x += 4) {
            if (x + 2 < graph_dimension) {
                batch_09.emplace_back(glm::uvec4(start_of_row + x, start_of_row + x + 2, 0, 1));
            }

            if (x + 3 < graph_dimension) {
                batch_09.emplace_back(glm::uvec4(start_of_row + x + 1, start_of_row + x + 3, 0, 1));
            }
        }

        for (int x = 2; x < graph_dimension; x += 4) {
            if (x + 2 < graph_dimension) {
                batch_10.emplace_back(glm::uvec4(start_of_row + x, start_of_row + x + 2, 0, 1));
            }

            if (x + 3 < graph_dimension) {
                batch_10.emplace_back(glm::uvec4(start_of_row + x + 1, start_of_row + x + 3, 0, 1));
            }
        }
    }

    std::vector<glm::uvec4> batch_11;
    std::vector<glm::uvec4> batch_12;
    for (int x = 0; x < graph_dimension; ++x) {
        for (int y = 0; y < graph_dimension; y += 4) {
            if (y + 2 < graph_dimension) {
                batch_11.emplace_back(glm::uvec4(y * graph_dimension + x, (y + 2) * graph_dimension + x, 0, 1));
            }

            if (y + 3 < graph_dimension) {
                batch_11.emplace_back(glm::uvec4((y + 1) * graph_dimension + x, (y + 3) * graph_dimension, 0, 1));
            }
        }

        for (int y = 2; y < graph_dimension; y += 4) {
            if (y + 2 < graph_dimension) {
                batch_12.emplace_back(glm::uvec4(y * graph_dimension + x, (y + 2) * graph_dimension + x, 0, 1));
            }

            if (y + 3 < graph_dimension) {
                batch_12.emplace_back(glm::uvec4((y + 1) * graph_dimension + x, (y + 3) * graph_dimension, 0, 1));
            }
        }
    }

    return { batch_01, batch_02, batch_03, batch_04, batch_05, batch_06, batch_07, batch_08 }; //, batch_09, batch_10, batch_11, batch_12 };
}

} // namespace BlackBirdBox
