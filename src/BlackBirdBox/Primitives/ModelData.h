#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>


namespace Core {
	
	struct Vertex {
		glm::vec3 pos;
		glm::vec3 normal;
		glm::vec2 tex_coord;

		bool operator==(const Vertex& other) const {
			return pos == other.pos && normal == other.normal && tex_coord == other.tex_coord;
		}
	};

	struct ModelData
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
	};

}

namespace std {
	template<> struct hash<Core::Vertex> {
		size_t operator()(Core::Vertex const& vertex) const {
			return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.normal) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.tex_coord) << 1);
		}
	};
}