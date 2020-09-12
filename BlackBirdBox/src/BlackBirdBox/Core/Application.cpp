#include "bbbpch.h"
#include "Application.h"

#include "BlackBirdBox/Renderer/Renderer.h"
#include "Input.h"
#include "Log.h"
#include "TimeStep.h"

#include <glfw/glfw3.h>

namespace Core {

Application* Application::s_Instance = nullptr;

Application::Application()
{
    PROFILE_FUNCTION();

    CORE_ASSERT(!s_Instance, "Application already exists!");
    s_Instance = this;
    m_Window = Window::Create();
    m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

    Renderer::Init();

    m_ImGuiLayer = new ImGuiLayer();
    PushOverlay(m_ImGuiLayer);
}

Application::~Application()
{
    PROFILE_FUNCTION();

    Renderer::Shutdown();
}

void Application::PushLayer(Layer* layer)
{
    PROFILE_FUNCTION();

    LOG_TRACE("Pushing Layer: {0}", layer->GetName());
    m_LayerStack.PushLayer(layer);
    layer->OnAttach();
}

void Application::PushOverlay(Layer* layer)
{
    PROFILE_FUNCTION();

    LOG_TRACE("Pushing Overlay: {0}", layer->GetName());
    m_LayerStack.PushOverlay(layer);
    layer->OnAttach();
}

void Application::OnEvent(Event& e)
{
    PROFILE_FUNCTION();

    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
    dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));
    dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(Application::OnKeyPressedEvent));

    for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
        (*it)->OnEvent(e);
        if (e.Handled)
            break;
    }
}

void Application::Run()
{

    PROFILE_FUNCTION();

    while (m_Running) {
        float time = (float)glfwGetTime();
        TimeStep timestep = time - m_LastFrameTime;
        m_LastFrameTime = time;

        if (!m_Minimized) {
            {
                for (Layer* layer : m_LayerStack)
                    layer->OnUpdate(timestep);
            }

            m_ImGuiLayer->Begin();
            {

                for (Layer* layer : m_LayerStack)
                    layer->OnImGuiRender();
            }
            m_ImGuiLayer->End();
        }

        m_Window->OnUpdate();
    }
}

bool Application::OnWindowClose(WindowCloseEvent& e)
{
    PROFILE_FUNCTION();

    m_Running = false;
    return true;
}

bool Application::OnWindowResize(WindowResizeEvent& e)
{

    PROFILE_FUNCTION();

    if (e.GetWidth() == 0 || e.GetHeight() == 0) {
        m_Minimized = true;
        return false;
    }

    m_Minimized = false;
    Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

    return false;
}

bool Application::OnKeyPressedEvent(KeyPressedEvent& e)
{
    PROFILE_FUNCTION();

    if (e.GetKeyCode() == KeyCode::Escape) {
        m_Running = false;
        return true;
    }

    return false;
}

}
