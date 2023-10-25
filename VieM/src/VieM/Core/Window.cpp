#include "Window.h"


#include "Application.h"
#include "VieM/ImGui/ImGuiTheme.h"
#include "VieM/UI/Utils.h"
#include "VieM/UI/UI.h"
#include "Log.h"
#include "VieM/Events/ApplicationEvent.h"
#include "VieM/Events/KeyEvent.h"
#include "VieM/Events/MouseEvent.h"


#include "stb_image.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>

#include <stdio.h>
#include <iostream>


static void glfw_error_callback(int error, const char* description)
{
	VM_ERROR("GLFW Error (", error, "): ", description);
}

namespace VieM {



	Window::Window(ApplicationSpecification& specs)
		: m_Specification(specs)
	{
		Init();
		m_TitleBar = std::make_shared<UI::TitleBar>(this);
		m_Settings = std::make_shared<UI::Settings>();
	}
	
	void Window::OnUpdate()
	{
		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.

		glfwPollEvents();
		m_Context->SwapBuffers();
	}
	

	
	void Window::SetVSync(bool enabled)
	{
		if (enabled)
		{
			VM_STATE("VSync enabled");
			glfwSwapInterval(1);
		}
		else
		{
			VM_STATE("VSync disabled");
			glfwSwapInterval(0);
		}

		m_Data.VSync = enabled;
	}
	
	bool Window::IsVSync() const
	{
		return m_Data.VSync;
	}

	bool Window::IsMaximized() const
	{
		return (bool)glfwGetWindowAttrib(m_Window, GLFW_MAXIMIZED);
	}

	GLFWwindow* Window::GetNativeWindow() const
	{
		return m_Window;
	}
	
	void Window::Init()
	{
		// Setup GLFW window
		glfwSetErrorCallback(glfw_error_callback);

		VM_INFO("Creating Window", m_Specification.Name, "(", m_Specification.Width, "x", m_Specification.Height, ")");

		int success = glfwInit();
		VM_ASSERT(success, "Could not initalize GLFW!");
		
		if (m_Specification.CustomTitleBar)
		{
			glfwWindowHint(GLFW_TITLEBAR, false);
			//glfwWindowHint(GLFW_DECORATED, false);
			glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);
		}

		GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);

		int monitorX, monitorY;
		glfwGetMonitorPos(primaryMonitor, &monitorX, &monitorY);
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);



		// Create window with graphics context
		m_Window = glfwCreateWindow(m_Specification.Width, m_Specification.Height, m_Specification.Name.c_str(), nullptr, nullptr);

		VM_ASSERT(m_Window, "Failed to create window");


		if (m_Specification.CenterWindow)
		{
			glfwSetWindowPos(m_Window,
				monitorX + (videoMode->width - m_Specification.Width) / 2,
				monitorY + (videoMode->height - m_Specification.Height) / 2);
			glfwSetWindowAttrib(m_Window, GLFW_RESIZABLE, m_Specification.WindowResizeable ? GLFW_TRUE : GLFW_FALSE);
		
		}
		glfwShowWindow(m_Window);

		m_Context = std::make_unique<GraphicsContext>(m_Window);
		m_Context->Init();

		SetVSync(true);

		// Set icon
		GLFWimage icon;
		int channels;
		if (!m_Specification.IconPath.empty())
		{
			std::string iconPathStr = m_Specification.IconPath.string();
			icon.pixels = stbi_load(iconPathStr.c_str(), &icon.width, &icon.height, &channels, 4);
			glfwSetWindowIcon(m_Window, 1, &icon);
			stbi_image_free(icon.pixels);
		}

		//glfwSetWindowUserPointer(m_Window, &Application::Get());
		glfwSetWindowUserPointer(m_Window, this);
		glfwSetTitlebarHitTestCallback(m_Window, [](GLFWwindow* window, int x, int y, int* hit)
			{
				Window* appWindow = (Window*)glfwGetWindowUserPointer(window);
				*hit = appWindow->m_TitleBar->IsHovered();
			});
		//glfwSetWindowUserPointer(m_Window, &m_Data);
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				Window& appWindow = *(Window*)glfwGetWindowUserPointer(window);
				WindowData& data = appWindow.m_Data;
				data.Width = width;
				data.Height = height;

				WindowResizeEvent event(width, height);
				data.EventCallback(event);
			});
#if 1		
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				Window& appWindow = *(Window*)glfwGetWindowUserPointer(window);
				WindowData& data = appWindow.m_Data;
				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent event((KeyCode)key);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event((KeyCode)key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event((KeyCode)key);
					data.EventCallback(event);
					break;
				}
				}
			});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
			{
				Window& appWindow = *(Window*)glfwGetWindowUserPointer(window);
				WindowData& data = appWindow.m_Data;
				KeyTypedEvent event((KeyCode)keycode);
				data.EventCallback(event);
			});
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				Window& appWindow = *(Window*)glfwGetWindowUserPointer(window);
				WindowData& data = appWindow.m_Data;
				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event((MouseButton)button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event((MouseButton)button);
					data.EventCallback(event);
					break;
				}
				}
			});
		
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				Window& appWindow = *(Window*)glfwGetWindowUserPointer(window);
				WindowData& data = appWindow.m_Data;

				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data.EventCallback(event);

			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
			{
				Window& appWindow = *(Window*)glfwGetWindowUserPointer(window);
				WindowData& data = appWindow.m_Data;

				MouseMovedEvent event((float)xPos, (float)yPos);
				data.EventCallback(event);
			});
#endif
		
	}
	
	void Window::Shutdown()
	{

	}
	

	void Window::DrawBase()
	{
		///////////////////////
		const bool isMaximized = IsMaximized();
		{
			ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(255, 50, 50, 255));
			// Draw window border if the window is not maximized
			if (!isMaximized)
				Utils::RenderWindowOuterBorders(ImGui::GetCurrentWindow());

			ImGui::PopStyleColor(); // ImGuiCol_Border
		}
		if (GetSpecification().CustomTitleBar)
		{
			float titleBarHeight;
			m_TitleBar->Draw(titleBarHeight);
			ImGui::SetCursorPosY(titleBarHeight);

		}
	}

}