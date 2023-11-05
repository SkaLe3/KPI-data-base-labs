#include "Application.h"

#include "VieM/ImGui/ImGuiContext.h"
#include "VieM/Render/Renderer.h"
#include "Log.h"


#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <glad/glad.h>
#include "GLFW/glfw3.h"


#include <iostream>

// temp
#include "VieM/UI/Utils.h"

extern bool g_ApplicationRunning;

namespace VieM {


	static Application* s_Instance = nullptr;

	Application::Application(const ApplicationSpecification& applicationSpecification /*= ApplicationSpecification()*/)
		: m_Specification(applicationSpecification)
	{
		Log::GetLogger()->Init();
		s_Instance = this;
		Init();
	}


	Application::~Application()
	{
		Shutdown();

		s_Instance = nullptr;
	}


	void Application::Run()
	{
		m_Running = true;
		Renderer::SetClearColor(0.2f, 0.2f, 0.2f, 1.0f);

		// Main loop
		while (!glfwWindowShouldClose(m_Window->GetNativeWindow()) && m_Running)
		{
			float time = (float)glfwGetTime();
			m_FrameTime = time - m_LastFrameTime;
			m_TimeStep = std::min<float>(m_FrameTime, 0.0333f);
			m_LastFrameTime = time;

			{
				std::scoped_lock<std::mutex> lock(m_EventQueueMutex);

				// Process custom event queue
				while (m_EventQueue.size() > 0)
				{
					auto& func = m_EventQueue.front();
					func();
					m_EventQueue.pop();
				}
			}


			for (auto& layer : m_LayerStack)
				layer->OnUpdate(m_TimeStep);

			Renderer::Clear();
			IGContext::BeginFrame();

			// Rendering 
			IGContext::EnableDocking(*m_Window);
			m_Window->DrawBase();
			IGContext::RenderDockspace(*m_Window);

			for (auto& layer : m_LayerStack)
				layer->OnUIRender();

			ImGui::End();

			IGContext::EndFrame();

			m_Window->OnUpdate();
		}
	}




	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResized));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (e.m_Handled)
				break;
			(*it)->OnEvent(e);
		}
	}

	Application& Application::GetName()
	{
		return *s_Instance;
	}

	void Application::Shutdown()
	{
		for (auto& layer : m_LayerStack)
			layer->OnDetach();

		m_LayerStack.clear();

		// Cleanup
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwDestroyWindow(m_Window->GetNativeWindow());
		glfwTerminate();

		g_ApplicationRunning = false;
	}

	void Application::Init()
	{
		m_Window = std::make_unique<Window>(m_Specification);
		VieM::IGContext::Init(GetWindow().GetNativeWindow());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

	}

	bool Application::OnWindowResized(WindowResizeEvent& e)
	{
		Renderer::OnWindowResized(e.GetWidth(), e.GetHeight());
		return false;
	}

}

	