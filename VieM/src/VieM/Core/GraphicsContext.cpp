#include "GraphicsContext.h"
#include "Log.h"

#include <Glad/glad.h>
#include "GLFW/glfw3.h"

namespace VieM {



	GraphicsContext::GraphicsContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
	{
		VM_ASSERT(windowHandle, "Window handle is null");
	}

	void GraphicsContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		VM_ASSERT(status, "Failed to initialize Glad!");
		VM_INFO("OpenGL Info:");
		VM_INFO("\tVendor: ", glGetString(GL_VENDOR));
		VM_INFO("\tRenderer: ", glGetString(GL_RENDERER));
		VM_INFO("\tVersion: ", glGetString(GL_VERSION));

		// make assert for minimal version
	}
	void GraphicsContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}