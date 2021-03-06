﻿#include "bbbpch.h"
#include "ModelLibrary.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

namespace BlackBirdBox {

void ModelLibrary::Add(const std::string& name, const ModelData& model)
{
    PROFILE_FUNCTION();

    CORE_ASSERT(!Exists(name), "Model already exists!");
    models_[name] = model;
}

const ModelData& ModelLibrary::Load(const std::string& filepath)
{
    PROFILE_FUNCTION();

    ModelData model = LoadModel(filepath);
    Add(filepath, model);
    return model;
}

const ModelData& ModelLibrary::Load(const std::string& name, const std::string& filepath)
{
    PROFILE_FUNCTION();

    ModelData model = LoadModel(filepath);
    Add(name, model);
    return model;
}

const ModelData& ModelLibrary::Get(const std::string& name)
{
    PROFILE_FUNCTION();

    CORE_ASSERT(Exists(name), "Model not found!");
    return models_[name];
}

bool ModelLibrary::Exists(const std::string& name) const
{
    PROFILE_FUNCTION();

    return models_.find(name) != models_.end();
}

ModelData ModelLibrary::LoadModel(const std::string& model_path)
{
    PROFILE_FUNCTION();

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
            vertex.pos = { attrib.vertices[3 * index.vertex_index + 0], attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2] };

            if (has_normals) {
                vertex.normal = {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2],
                };
            }

            if (has_texture_cordinates) {
                vertex.tex_coord = { attrib.texcoords[2 * index.texcoord_index + 0], attrib.texcoords[2 * index.texcoord_index + 1] };
            }

            if (unique_vertices.count(vertex) == 0) { // TODO(Rok Kos): Check if this is true: There probably aren't two vertexes with same positions
                                                      // and different texture cordinates
                unique_vertices[vertex] = static_cast<uint32_t>(model_data.vertices.size());
                model_data.vertices.push_back(vertex);
            }

            model_data.indices.push_back(unique_vertices[vertex]);
        }
    }

    return model_data;
}

}
