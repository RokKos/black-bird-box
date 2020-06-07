#pragma once

#include "BlackBirdBox.h"

#include <glm/glm.hpp>

namespace EOL {

	class EOLLayer : public Core::Layer
	{
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

	private:
		Core::TimeStep prev_time_step_;
		glm::vec4 bg_color_ = { 0.1f, 0.1f, 0.1f, 1 };
		
		Core::ShaderLibrary shader_library_;
		
		Core::PerspectiveCameraController perspective_camera_controller_;

		Core::Scene scene_;

		Core::Ref<Core::ShaderStorageArray> cloth_storage_array_;
		Core::Ref<Core::VertexArray> vertex_array_box_;
		Core::Ref<Core::Shader> enviroment_map_shader_;
		Core::Ref<Core::CubeMap> enviroment_map_;
		Core::ComputeShaderConfiguration compute_shader_configuration_;
		unsigned int num_cloth_dimension_size_;
		unsigned int num_cloth_particles_;

		int polygon_mode_ = 1;
		const char* polygon_mode_names_[3]{ "Point", "Line", "Fill" };
		
	};
}