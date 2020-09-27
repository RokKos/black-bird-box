#include "bbbpch.h"

#include "LightSource.h"

namespace BlackBirdBox {
const Ref<Transform>& LightSource::GetTransform() const { return transform_; }

const glm::vec3& LightSource::GetPosition() const { return transform_->GetPosition(); }

const glm::vec3& LightSource::GetDirection() const { return transform_->GetRotation(); }

const glm::vec3& LightSource::GetIntensity() const { return intensity_; }

const glm::vec3& LightSource::GetColor() const { return color_; }

const std::string& LightSource::GetName() const { return name_; }

bool LightSource::GetObjectEnabled() const { return enabled_; }

bool* LightSource::GetObjectEnabledImGui() { return &enabled_; }

void LightSource::SetObjectEnabled(bool enabled) { enabled_ = enabled; }

void LightSource::SetPosition(const glm::vec3& pos) { transform_->SetPosition(pos); }

void LightSource::SetDirection(const glm::vec3& dir) { transform_->SetRotation(dir); }

void LightSource::SetIntensity(const glm::vec3& intensity) { intensity_ = intensity; }

void LightSource::SetColor(const glm::vec3& color) { color_ = color; }
}
