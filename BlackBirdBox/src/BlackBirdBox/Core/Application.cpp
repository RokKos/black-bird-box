#include "bbbpch.h"
#include "Application.h"

#include "BlackBirdBox/Renderer/Renderer.h"
#include "Input.h"
#include "Log.h"
#include "TimeStep.h"

#include <glfw/glfw3.h>

namespace BlackBirdBox {

Application* Application::s_Instance = nullptr;

Application::Application()
{
    PROFILE_FUNCTION();

    CORE_ASSERT(!s_Instance, "Application already exists!");
    s_Instance = this;
    window_ = Window::Create();
    window_->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

    Renderer::Init();

    im_gui_layer_ = CreateRef<ImGuiLayer>();
    PushOverlay(im_gui_layer_);
}

Application::~Application()
{
    PROFILE_FUNCTION();

    Renderer::Shutdown();
}

void Application::Close() { running_ = false; }

void Application::PushLayer(const Ref<Layer>& layer)
{
    PROFILE_FUNCTION();

    LOG_TRACE("Pushing Layer: {0}", layer->GetName());
    layer_stack_.PushLayer(layer);
    layer->OnAttach();
}

void Application::PopLayer(const Ref<Layer>& layer)
{
    PROFILE_FUNCTION();

    LOG_TRACE("Poping Layer: {0}", layer->GetName());
    layer_stack_.PopLayer(layer);
}

void Application::PushOverlay(const Ref<Layer>& layer)
{
    PROFILE_FUNCTION();

    LOG_TRACE("Pushing Overlay: {0}", layer->GetName());
    layer_stack_.PushOverlay(layer);
    layer->OnAttach();
}

const Ref<ImGuiLayer>& Application::GetImGuiLayer() const { return im_gui_layer_; }

void Application::OnEvent(Event& e)
{
    PROFILE_FUNCTION();

    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
    dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));
    dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(Application::OnKeyPressedEvent));

    for (auto it = layer_stack_.rbegin(); it != layer_stack_.rend(); ++it) {
        (*it)->OnEvent(e);
        if (e.Handled)
            break;
    }
}

void Application::Run()
{

    PROFILE_FUNCTION();

    while (running_) {
        float time = (float)glfwGetTime();
        TimeStep timestep = time - last_frame_time_;
        last_frame_time_ = time;

        if (!minimized_) {
            if (!are_layers_paused) {
                for (const Ref<Layer>& layer : layer_stack_)
                    layer->OnUpdate(timestep);
            }

            im_gui_layer_->Begin();
            {

                for (const Ref<Layer>& layer : layer_stack_)
                    layer->OnImGuiRender();
            }
            im_gui_layer_->End();
        }

        window_->OnUpdate();
    }
}

bool Application::OnWindowClose(WindowCloseEvent& e)
{
    PROFILE_FUNCTION();

    Close();
    return true;
}

bool Application::OnWindowResize(WindowResizeEvent& e)
{

    PROFILE_FUNCTION();

    if (e.GetWidth() == 0 || e.GetHeight() == 0) {
        minimized_ = true;
        return false;
    }

    minimized_ = false;
    Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

    return false;
}

bool Application::OnKeyPressedEvent(KeyPressedEvent& e)
{
    PROFILE_FUNCTION();

    if (e.GetKeyCode() == KeyCode::Escape) {
        Close();
        return true;
    }

    if (e.GetKeyCode() == KeyCode::F5) {
        are_layers_paused = !are_layers_paused;
        return true;
    }

    return false;
}

}
