#include "bbbpch.h"
#include "WindowsWindow.h"

#include "BlackBirdBox/Core/Input.h"

#include "BlackBirdBox/Events/ApplicationEvent.h"
#include "BlackBirdBox/Events/KeyEvent.h"
#include "BlackBirdBox/Events/MouseEvent.h"

#include "BlackBirdBox/Renderer/Renderer.h"

namespace Platform {

static uint8_t s_GLFWWindowCount = 0;

static void GLFWErrorCallback(int error, const char* description) { LOG_ERROR("GLFW Error ({0}): {1}", error, description); }

WindowsWindow::WindowsWindow(const BlackBirdBox::WindowProps& props)
{
    PROFILE_FUNCTION();

    Init(props);
}

WindowsWindow::~WindowsWindow()
{
    PROFILE_FUNCTION();

    Shutdown();
}

void WindowsWindow::Init(const BlackBirdBox::WindowProps& props)
{
    PROFILE_FUNCTION();

    m_Data.Title = props.Title;
    m_Data.Width = props.Width;
    m_Data.Height = props.Height;

    LOG_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

    if (s_GLFWWindowCount == 0) {
        int success = glfwInit();
        CORE_ASSERT(success, "Could not intialize GLFW!");
        glfwSetErrorCallback(GLFWErrorCallback);
    }

    {

        if (BlackBirdBox::Renderer::GetAPI() == BlackBirdBox::RendererAPI::API::OpenGL) {
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
        }

        m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
        if (!m_Window) {
            glfwTerminate();
            return;
        }
        ++s_GLFWWindowCount;
    }

    m_Context = BlackBirdBox::GraphicsContext::Create(m_Window);
    m_Context->Init();

    glfwSetWindowUserPointer(m_Window, &m_Data);
    SetVSync(false);

    // Set GLFW callbacks
    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        data.Width = width;
        data.Height = height;

        BlackBirdBox::WindowResizeEvent event(width, height);
        data.EventCallback(event);
    });

    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        BlackBirdBox::WindowCloseEvent event;
        data.EventCallback(event);
    });

    glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        switch (action) {
        case GLFW_PRESS: {
            BlackBirdBox::KeyPressedEvent event(static_cast<BlackBirdBox::KeyCode>(key), 0);
            data.EventCallback(event);
            break;
        }
        case GLFW_RELEASE: {
            BlackBirdBox::KeyReleasedEvent event(static_cast<BlackBirdBox::KeyCode>(key));
            data.EventCallback(event);
            break;
        }
        case GLFW_REPEAT: {
            BlackBirdBox::KeyPressedEvent event(static_cast<BlackBirdBox::KeyCode>(key), 1);
            data.EventCallback(event);
            break;
        }
        }
    });

    glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        BlackBirdBox::KeyTypedEvent event(static_cast<BlackBirdBox::KeyCode>(keycode));
        data.EventCallback(event);
    });

    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        switch (action) {
        case GLFW_PRESS: {
            BlackBirdBox::MouseButtonPressedEvent event(static_cast<BlackBirdBox::MouseCode>(button));
            data.EventCallback(event);
            break;
        }
        case GLFW_RELEASE: {
            BlackBirdBox::MouseButtonReleasedEvent event(static_cast<BlackBirdBox::MouseCode>(button));
            data.EventCallback(event);
            break;
        }
        }
    });

    glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        BlackBirdBox::MouseScrolledEvent event((float)xOffset, (float)yOffset);
        data.EventCallback(event);
    });

    glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        BlackBirdBox::MouseMovedEvent event((float)xPos, (float)yPos);
        data.EventCallback(event);
    });
}

void WindowsWindow::Shutdown()
{
    PROFILE_FUNCTION();

    glfwDestroyWindow(m_Window);
    --s_GLFWWindowCount;

    if (s_GLFWWindowCount == 0) {
        glfwTerminate();
    }
}

void WindowsWindow::OnUpdate()
{
    PROFILE_FUNCTION();

    glfwPollEvents();
    m_Context->SwapBuffers();
}

void WindowsWindow::SetVSync(bool enabled)
{
    PROFILE_FUNCTION();

    if (enabled)
        glfwSwapInterval(1);
    else
        glfwSwapInterval(0);

    m_Data.VSync = enabled;
}

bool WindowsWindow::IsVSync() const
{
    PROFILE_FUNCTION();

    return m_Data.VSync;
}

}
