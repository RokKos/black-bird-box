#pragma once

#include "Core.h"

#include "BlackBirdBox/Events/ApplicationEvent.h"
#include "BlackBirdBox/Events/Event.h"
#include "BlackBirdBox/Layer/LayerStack.h"
#include "Window.h"

#include "BlackBirdBox/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace BlackBirdBox {

class Application {
public:
    Application();
    virtual ~Application();

    void Close();

    virtual void OnEvent(Event& e);

    void PushLayer(const Ref<Layer>& layer);
    void PopLayer(const Ref<Layer>& layer);
    void PushOverlay(const Ref<Layer>& layer);

    const Ref<ImGuiLayer>& GetImGuiLayer() const;

    Window& GetWindow();

    static Application& Get();

private:
    void Run();
    bool OnWindowClose(WindowCloseEvent& e);
    bool OnWindowResize(WindowResizeEvent& e);
    bool OnKeyPressedEvent(KeyPressedEvent& e);

private:
    Scope<Window> window_;
    Ref<ImGuiLayer> im_gui_layer_;
    bool running_ = true;
    bool are_layers_paused = false;
    bool minimized_ = false;
    LayerStack layer_stack_;
    float last_frame_time_ = 0.0f;

private:
    static Application* s_Instance; // TODO(Rok Kos): Move this to smart pointer if possible
    friend int ::main(int argc, char** argv);
};

// To be defined in CLIENT
Application* CreateApplication();

}
