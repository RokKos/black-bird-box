#pragma once

#include "BlackBirdBox/Layer/Layer.h"

#include "BlackBirdBox/Events/ApplicationEvent.h"
#include "BlackBirdBox/Events/KeyEvent.h"
#include "BlackBirdBox/Events/MouseEvent.h"

namespace Core {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};

}