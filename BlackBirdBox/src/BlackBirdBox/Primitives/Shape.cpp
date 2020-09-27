#include "bbbpch.h"
#include "Shape.h"

namespace BlackBirdBox {

Shape::Shape(const Ref<Material>& material, const Ref<VertexArray>& vertex_array, const Ref<Transform>& transform, const ModelData& model_data,
    const std::string& name)
    : material_(material)
    , vertex_array_(vertex_array)
    , model_data_(model_data)
    , transform_(transform)
    , name_(name)
    , enabled_(true)
{
    PROFILE_FUNCTION();
}

}
