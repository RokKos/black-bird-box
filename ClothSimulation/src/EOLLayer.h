#pragma once

#include "BlackBirdBox.h"

#include <glm/glm.hpp>

namespace EOL {

class EOLLayer : public Core::Layer {
public:
    EOLLayer();
    virtual ~EOLLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    void OnUpdate(Core::TimeStep ts) override;
    virtual void OnImGuiRender() override;
    void OnEvent(Core::Event& e) override;

private:
    bool OnKeyPressedEvent(Core::KeyPressedEvent& e);
    bool OnKeyTypedEvent(Core::KeyTypedEvent& e);

    void ParseSimulationSettings();
    void LoadAllShaders();
    void LoadAllPrimitiveModels();
    void LoadModelInThread(const std::string& filepath);
    std::string FrameBufferAttachmentToName(Core::FrameBufferAttachments attachment);

private:
    Core::TimeStep prev_time_step_;
    glm::vec4 bg_color_ = { 0.1f, 0.1f, 0.1f, 1 };

    Core::ShaderLibrary shader_library_;
    Core::ModelLibrary model_library_;

    Core::Ref<Core::PerspectiveCameraController> perspective_camera_controller_;

    std::vector<Core::Ref<Core::BaseMenu>> menus_;

    Core::Scene scene_;

    Core::Ref<Core::VertexArray> vertex_array_box_;
    Core::Ref<Core::CubeMap> enviroment_map_;
    Core::Ref<Core::FrameBuffer> test_frame_buffer_;
    Core::Ref<Core::Shape> frame_buffer_obj_;
    Core::Ref<Core::Cloth> cloth_;
    Core::ComputeShaderConfiguration compute_shader_configuration_;
    Core::ComputeShaderSimulationConfiguration compute_shader_simulation_configuration_;
    glm::vec4 fixed_point_to_move_ = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
    unsigned int num_cloth_dimension_size_;

    int polygon_mode_ = 1;
    const char* polygon_mode_names_[3]{ "Point", "Line", "Fill" };
};
}
