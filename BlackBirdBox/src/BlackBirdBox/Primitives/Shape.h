#pragma once

#include "BlackBirdBox/Components/Material.h"
#include "BlackBirdBox/Components/Transform.h"
#include "BlackBirdBox/Renderer/Buffers/VertexArray.h"
#include "ModelData.h"

namespace BlackBirdBox {

class Shape {
public:
    Shape() = default;

    Shape(const Ref<Material>& material, const Ref<VertexArray>& vertex_array, const Ref<Transform>& transform,
        const ModelData& model_data = ModelData(), const std::string& name = "shape");

    const Ref<Material>& GetMaterial() const { return material_; }
    const Ref<VertexArray>& GetVertexArray() const { return vertex_array_; }
    const Ref<Transform>& GetTransform() const { return transform_; }
    const std::string& GetName() const { return name_; }
    bool GetObjectEnabled() const { return enabled_; }
    bool* GetObjectEnabledImGui() { return &enabled_; }
    void SetObjectEnabled(bool enabled) { enabled_ = enabled; }

protected:
    Ref<VertexArray> vertex_array_;

private:
    std::string name_ = "shape";
    bool enabled_ = true;

    Ref<Material> material_;
    ModelData model_data_ = ModelData();
    Ref<Transform> transform_;
};
}
