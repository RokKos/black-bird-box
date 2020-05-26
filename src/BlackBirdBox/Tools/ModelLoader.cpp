#include "bbbpch.h"
#include "ModelLoader.h"

#include "BlackBirdBox/Core/Core.h"
#include "BlackBirdBox/Core/Log.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

namespace Core {

	ModelData ModelLoader::LoadModel(const std::string& model_path)
	{
		ModelData model_data;
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn, err;

		bool is_object_not_loaded = !tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, model_path.c_str());
		if (is_object_not_loaded) {
			LOG_WARN("{0}", warn);
			LOG_ERROR("{0}", err);
			CORE_ASSERT(false, "Object {0} was not loaded succesfully", model_path.c_str())
		}

		LOG_WARN("{0}", warn);

		std::unordered_map<Vertex, uint32_t> unique_vertices = {};

		bool has_texture_cordinates = attrib.texcoords.size() > 0;
		bool has_normals = attrib.normals.size() > 0;

		for (const auto& shape : shapes) {
			for (const auto& index : shape.mesh.indices) {
				Vertex vertex{};
				vertex.pos = { 
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2] 
				};
				
				if (has_normals) {
					vertex.normal = {
						attrib.normals[3 * index.normal_index + 0],
						attrib.normals[3 * index.normal_index + 1],
						attrib.normals[3 * index.normal_index + 2],
					};
				}

				if (has_texture_cordinates) {
					vertex.tex_coord = {
						attrib.texcoords[2 * index.texcoord_index + 0],
						attrib.texcoords[2 * index.texcoord_index + 1]
					};
				}

				if (unique_vertices.count(vertex) == 0) {  // TODO(Rok Kos): Check if this is true: There probably aren't two vertexes with same positions and different texture cordinates
					unique_vertices[vertex] = static_cast<uint32_t>(model_data.vertices.size());
					model_data.vertices.push_back(vertex);
				}

				model_data.indices.push_back(unique_vertices[vertex]);
			}
		}

		return model_data;
	}

}

