#pragma once

#include "BlackBirdBox/Core/Core.h"
#include "BlackBirdBox/Core/Window.h"
#include "BlackBirdBox/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Platform {

class WindowsWindow : public BlackBirdBox::Window {
public:
    WindowsWindow(const BlackBirdBox::WindowProps& props);
    virtual ~WindowsWindow();

    void OnUpdate() override;

    inline unsigned int GetWidth() const override { return m_Data.Width; }
    inline unsigned int GetHeight() const override { return m_Data.Height; }

    // Window attributes
    inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
    void SetVSync(bool enabled) override;
    bool IsVSync() const override;

    inline virtual void* GetNativeWindow() const { return m_Window; }

private:
    virtual void Init(const BlackBirdBox::WindowProps& props);
    virtual void Shutdown();

private:
    GLFWwindow* m_Window;
    BlackBirdBox::Scope<BlackBirdBox::GraphicsContext> graphics_context_;

    struct WindowData {
        std::string Title;
        unsigned int Width, Height;
        bool VSync;

        EventCallbackFn EventCallback;
    };

    WindowData m_Data;
};

}
