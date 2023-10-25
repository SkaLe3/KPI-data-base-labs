#pragma once
#include "Window.h"
#include "Layer.h"
#include "Image.h"
#include "VieM/UI/UI.h"
#include "VieM/Events/ApplicationEvent.h"



#include <filesystem>
#include <string>
#include <queue>
#include <imgui.h>

struct GLFWwindow;


namespace VieM {

	class ImGuiContext;

	struct ApplicationSpecification
	{
		std::string Name = "VieM Framework";
		uint32_t Width = 1600;
		uint32_t Height = 900;

		std::filesystem::path IconPath;

		bool WindowResizeable = true;
		bool CustomTitleBar = false;
		bool CenterWindow = false;
	};


	class Application
	{
	public:
		Application(const ApplicationSpecification& applicationSpecification = ApplicationSpecification());
		~Application();

		void Run();

		void PushLayer(const std::shared_ptr<Layer>& layer) { m_LayerStack.emplace_back(layer); layer->OnAttach(); }

		void Close() { m_Running = false; }
		
		void OnEvent(Event& e);
		
		ApplicationSpecification& GetSpecifications() { return m_Specification; }

		Window& GetWindow() const { return *m_Window; }
		static Application& Get();

		template<typename Func>
		void QueueEvent(Func&& func)
		{
			m_EventQueue.push(func);
		}

	private:
		void Shutdown();
		void Init();

		bool OnWindowResized(WindowResizeEvent& e);

	private:
		ApplicationSpecification m_Specification;
		std::unique_ptr<Window> m_Window;

		bool m_Running = false;

		float m_TimeStep = 0.0f;
		float m_FrameTime = 0.0f;
		float m_LastFrameTime = 0.0f;

		std::vector<std::shared_ptr<Layer>> m_LayerStack;

		std::mutex m_EventQueueMutex;
		std::queue<std::function<void()>> m_EventQueue;

		


	};



	// Implemented by CLIENT
	Application* CreateApplication(int argc, char** argv);
}