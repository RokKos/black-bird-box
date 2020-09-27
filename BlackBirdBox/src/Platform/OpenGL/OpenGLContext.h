#pragma once

#include "BlackBirdBox/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Platform {

class OpenGLContext : public BlackBirdBox::GraphicsContext {
public:
    OpenGLContext(GLFWwindow* windowHandle);

    virtual void Init() override;
    virtual void SwapBuffers() override;

    virtual const std::array<int, 3>& GetWorkGroupSize() const override { return work_group_size_; };
    virtual const std::array<int, 3>& GetLocalGroupSize() const override { return local_group_size_; };
    virtual int GetMaxComputeShaderInvocations() const override { return max_compute_shader_invocations_; };

private:
    GLFWwindow* m_WindowHandle;

    std::array<int, 3> work_group_size_;
    std::array<int, 3> local_group_size_;
    int max_compute_shader_invocations_;
};

}
