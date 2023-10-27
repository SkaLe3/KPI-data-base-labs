#pragma once
#include <string>
#include <functional>


#include "GraphicsContext.h"
#include "VieM/Core/Image.h"
#include "VieM/Events/Event.h"
#include "VieM/UI/UI.h"

struct GLFWwindow;

namespace VieM {

	struct ApplicationSpecification;

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		Window(ApplicationSpecification& specs);
		~Window() { Shutdown(); }
		void OnUpdate();

		uint32_t GetWidth() const { return m_Data.Width; }
		uint32_t GetHeight() const { return m_Data.Height; }
		std::pair<int32_t, int32_t> GetPos() const;
		void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }

		void SetVSync(bool enabled);
		bool IsVSync() const;

		bool IsMaximized() const;

		GLFWwindow* GetNativeWindow() const;

		//std::shared_ptr<Image> GetApplicationIcon() const { return m_TitleBar.m_AppHeaderIcon; }
		

		UI::Settings& GetSettings() { return *m_Settings; };
		UI::TitleBar& GetTitlebar() { return *m_TitleBar; };
		ApplicationSpecification& GetSpecification() { return m_Specification; }

		void DrawBase();


	private:
		void Init();
		void Shutdown();
	private:
		ApplicationSpecification& m_Specification;
		GLFWwindow* m_Window;
		std::unique_ptr<GraphicsContext> m_Context;
		struct WindowData
		{
			std::string Title = "";
			unsigned int Width = 0, Height = 0;
			
			bool VSync;
			EventCallbackFn EventCallback;
		};

		WindowData m_Data;

	private:
		// std::fucntion<void()> m_TitlebarSettings / m_DrawTitlebar

		std::shared_ptr<UI::Settings> m_Settings;
		std::shared_ptr<UI::TitleBar> m_TitleBar;


	};
}