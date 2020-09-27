#pragma once

#include "BlackBirdBox/Components/Transform.h"
#include "BlackBirdBox/Core/Core.h"
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

    const Ref<Transform>& GetTransform() const;
    const glm::vec3& GetPosition() const;
    const glm::vec3& GetDirection() const;
    const glm::vec3& GetIntensity() const;
    const glm::vec3& GetColor() const;
    const std::string& GetName() const;
    bool GetObjectEnabled() const;
    bool* GetObjectEnabledImGui();
    void SetObjectEnabled(bool enabled);

    void SetPosition(const glm::vec3& pos);
    void SetDirection(const glm::vec3& dir);
    void SetIntensity(const glm::vec3& intensity);
    void SetColor(const glm::vec3& color);

private:
    std::string name_ = "Light Source";
    bool enabled_ = true;

    LightType type_ = LightType::kNone;
    Ref<Transform> transform_; // Pos = Pos, Rotation = Light Direction, Scale = Light Intensity
    glm::vec3 color_;
    glm::vec3 intensity_;
};
}
