#include "ImGuiContext.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>

// Embedded font
#include "Roboto-Regular.embed"
#include "Roboto-Bold.embed"
#include "Roboto-Italic.embed"

#include "VieM/UI/Utils.h"
#include "VieM/ImGui/ImGuiTheme.h"
#include "VieM/Core/Window.h"
#include "VieM/Core/Application.h"

#include <unordered_map>
#include <chrono>




namespace VieM {


	void IGContext::Init(GLFWwindow* window)
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;


		ImGui::StyleColorsDark(); 
		Utils::SetDefaultTheme();


		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");

		// Make like Settings
		//style.WindowPadding = ImVec2(10.0f, 10.0f);
		//style.FramePadding = ImVec2(8.0f, 6.0f);
		//style.ItemSpacing = ImVec2(6.0f, 6.0f);
		//style.ChildRounding = 6.0f;
		//style.PopupRounding = 6.0f;
		//style.FrameRounding = 6.0f;
		//style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
		 
		auto& settings = Application::Get().GetWindow().GetSettings();
		// Load default font
		ImFontConfig fontConfig;
		fontConfig.FontDataOwnedByAtlas = false;
		ImFont* robotoFont = io.Fonts->AddFontFromMemoryTTF((void*)g_RobotoRegular, sizeof(g_RobotoRegular), 20.0f, &fontConfig);
		settings.GetFontsStorage()["Roboto-Regular"] = robotoFont;
		settings.GetFontsStorage()["Roboto-Bold"] = io.Fonts->AddFontFromMemoryTTF((void*)g_RobotoBold, sizeof(g_RobotoBold), 20.0f, &fontConfig);
		settings.GetFontsStorage()["Roboto-Italic"] = io.Fonts->AddFontFromMemoryTTF((void*)g_RobotoItalic, sizeof(g_RobotoItalic), 20.0f, &fontConfig);
		io.FontDefault = robotoFont;



	}

	void IGContext::EnableDocking(Window& window)
	{
		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
				// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		if (!window.GetSpecification().CustomTitleBar && window.GetTitlebar().GetMenubarCallback())
			window_flags |= ImGuiWindowFlags_MenuBar;

		const bool isMaximized = window.IsMaximized();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, isMaximized ? ImVec2(6.0f, 6.0f) : ImVec2(1.0f, 1.0f)); // TODO: Make customizable
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.0f); // TODO: Make customizable

		ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f });
		ImGui::Begin("DockSpaceWindow", nullptr, window_flags);
		ImGui::PopStyleColor(); // MenuBarBg
		ImGui::PopStyleVar(2);

		ImGui::PopStyleVar(2);
	}

	void IGContext::RenderDockspace(Window& window)
	{


		// Dockspace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 250.0f; // Make customizable
		ImGui::DockSpace(ImGui::GetID("MyDockspace"));
		style.WindowMinSize.x = minWinSizeX;

		if (!window.GetSpecification().CustomTitleBar)
			window.GetTitlebar().DrawMenubar();
	}

	void IGContext::BeginFrame()
	{
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void IGContext::EndFrame()
	{
		ImGuiIO& io = ImGui::GetIO();

		ImGui::Render();
		ImDrawData* main_draw_data = ImGui::GetDrawData();
		const bool main_is_minimized = (main_draw_data->DisplaySize.x <= 0.0f || main_draw_data->DisplaySize.y <= 0.0f);

		if (!main_is_minimized)
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		// Present Main Platform Window
		if (main_is_minimized)
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		

	}

	
	
}