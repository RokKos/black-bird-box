#pragma once

#include "BlackBirdBox.h"

#include <glm/glm.hpp>

namespace BlackBirdNest {

class BlackBirdNestLayer : public BlackBirdBox::Layer {
public:
    BlackBirdNestLayer();
    virtual ~BlackBirdNestLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    void OnUpdate(BlackBirdBox::TimeStep ts) override;
    virtual void OnImGuiRender() override;
    void OnEvent(BlackBirdBox::Event& e) override;

private:
    void LoadAllShaders();
    void LoadAllPrimitiveModels();
    void LoadModelInThread(const std::string& filepath);
    std::string FrameBufferAttachmentToName(BlackBirdBox::FrameBufferAttachments attachment);
    void SetPhongParameters(
        const BlackBirdBox::Ref<BlackBirdBox::Material>& material, BlackBirdBox::PhongLightingParameters phong_lighting_parameters);

private:
    BlackBirdBox::TimeStep prev_time_step_;
    glm::vec4 bg_color_ = { 0.1f, 0.1f, 0.1f, 1 };

    BlackBirdBox::ShaderLibrary shader_library_;
    BlackBirdBox::ModelLibrary model_library_;

    BlackBirdBox::Ref<BlackBirdBox::PerspectiveCameraController> perspective_camera_controller_;

    std::vector<BlackBirdBox::Ref<BlackBirdBox::BaseMenu>> menus_;

    BlackBirdBox::Scene scene_;

    BlackBirdBox::Ref<BlackBirdBox::VertexArray> vertex_array_box_;
    BlackBirdBox::Ref<BlackBirdBox::CubeMap> enviroment_map_;
    BlackBirdBox::Ref<BlackBirdBox::Material> enviroment_map_material_;
    BlackBirdBox::Ref<BlackBirdBox::FrameBuffer> test_frame_buffer_;
    BlackBirdBox::Ref<BlackBirdBox::Shape> frame_buffer_obj_;

    BlackBirdBox::Ref<BlackBirdBox::AudioSample> sample_;

    int polygon_mode_ = 1;
    const char* polygon_mode_names_[3]{ "Point", "Line", "Fill" };
};
}
