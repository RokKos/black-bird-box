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

    void OnEvent(Event& e);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);

    inline Window& GetWindow() { return *m_Window; }

    inline static Application& Get() { return *s_Instance; }

private:
    void Run();
    bool OnWindowClose(WindowCloseEvent& e);
    bool OnWindowResize(WindowResizeEvent& e);
    bool OnKeyPressedEvent(KeyPressedEvent& e);

private:
    Scope<Window> m_Window;
    ImGuiLayer* m_ImGuiLayer; // TODO(Rok Kos): Move this to smart pointer if possible
    bool m_Running = true;
    bool m_Minimized = false;
    LayerStack m_LayerStack;
    float m_LastFrameTime = 0.0f;

private:
    static Application* s_Instance; // TODO(Rok Kos): Move this to smart pointer if possible
    friend int ::main(int argc, char** argv);
};

// To be defined in CLIENT
Application* CreateApplication();

}
