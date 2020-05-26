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

		Core::Ref<Core::ShaderStorageArray> particles_storage_array_;
		Core::ComputeShaderConfiguration compute_shader_configuration_;
		unsigned int num_particles_;
	};
}