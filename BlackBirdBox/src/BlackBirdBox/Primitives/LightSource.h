#pragma once

#include "BlackBirdBox/Components/Transform.h"
#include "glm/glm.hpp"

namespace BlackBirdBox {

enum class LightType : unsigned char {
    kNone = 0,
    kDirectional = 1,
    kPoint = 2,
    kReflector = 3,
};

class LightSource {
public:
    LightSource() = default;

    LightSource(LightType type, const Ref<Transform>& transform, glm::vec3 color, glm::vec3 intensity, const std::string& name = "Light Source")
        : type_(type)
        , transform_(transform)
        , color_(color)
        , name_(name)
        , intensity_(intensity){};

    const Ref<Transform>& GetTransform() const { return transform_; }
    const glm::vec3& GetPosition() const { return transform_->GetPosition(); }
    const glm::vec3& GetDirection() const { return transform_->GetRotation(); }
    const glm::vec3& GetIntensity() const { return intensity_; }
    const glm::vec3& GetColor() const { return color_; }
    const std::string& GetName() const { return name_; }
    bool GetObjectEnabled() const { return enabled_; }
    bool* GetObjectEnabledImGui() { return &enabled_; }
    void SetObjectEnabled(bool enabled) { enabled_ = enabled; }

    void SetPosition(const glm::vec3& pos) { transform_->SetPosition(pos); }
    void SetDirection(const glm::vec3& dir) { transform_->SetRotation(dir); }
    void SetIntensity(const glm::vec3& intensity) { intensity_ = intensity; }
    void SetColor(const glm::vec3& color) { color_ = color; }

private:
    std::string name_ = "Light Source";
    bool enabled_ = true;

    LightType type_ = LightType::kNone;
    Ref<Transform> transform_; // Pos = Pos, Rotation = Light Direction, Scale = Light Intensity
    glm::vec3 color_;
    glm::vec3 intensity_;
};

}
