#include "bbbpch.h"
#include "OpenGLContext.h"

#include "BlackBirdBox/Core/Log.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Platform {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		PROFILE_FUNCTION();

		CORE_ASSERT(windowHandle, "Window handle is null!")
	}

	void OpenGLContext::Init()
	{
		PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			LOG_WARN("Warning: Failed to initialize GLEW");
		}

		LOG_INFO("OpenGL version: {0}", glGetString(GL_VERSION));
		LOG_INFO("GLSL version: {0}", glGetString(GL_SHADING_LANGUAGE_VERSION));


		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_group_size_[0]);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_group_size_[1]);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_group_size_[2]);
		LOG_INFO("Max Global (total) Work Group counts x:{0} y:{1} z:{2}",
			work_group_size_[0], work_group_size_[1], work_group_size_[2]);

		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &local_group_size_[0]);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &local_group_size_[1]);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &local_group_size_[2]);
		LOG_INFO("Max Local (in one shader) work group sizes x:{0} y:{1} z:{2}",
			local_group_size_[0], local_group_size_[1], local_group_size_[2]);

		glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &max_compute_shader_invocations_);
		LOG_INFO("max local work group invocations {0}", max_compute_shader_invocations_);


		/*
		
		 int res;
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &res);
	printf("Max Color Attachments: %d\n", res);

	glGetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH, &res);
	printf("Max Framebuffer Width: %d\n", res);

	glGetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT, &res);
	printf("Max Framebuffer Height: %d\n", res);

	glGetIntegerv(GL_MAX_FRAMEBUFFER_SAMPLES, &res);
	printf("Max Framebuffer Samples: %d\n", res);

	glGetIntegerv(GL_MAX_FRAMEBUFFER_LAYERS, &res);
	printf("Max Framebuffer Layers: %d\n", res);
		
		*/
	}

	void OpenGLContext::SwapBuffers()
	{
		PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}

}