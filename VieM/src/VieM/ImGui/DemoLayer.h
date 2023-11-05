#pragma once

#include "VieM/Core/Layer.h"
#include "VieM/Core/Application.h"
#include "VieM/Render/Renderer.h"

#include "VieM/Core/Image.h"

#include "VieM/UI/Theme.h"
#include "VieM/Serialization/ThemeSerializer.h"

#include "VieM/Events/Event.h"

#include "imgui.h"
// temp 
#include <iostream>

//
namespace VieM {


	class DemoLayer : public VieM::Layer
	{
	public:
		virtual void OnAttach() override
		{
			Application* app = &Application::GetName();
			image = std::make_unique<Image>("assets/images/cat.png");
			UI::Theme* defaultTheme = app->GetWindow().GetSettings().GetCurrentTheme();
			editTheme = new VieM::UI::Theme(*defaultTheme);
			app->GetWindow().GetSettings().AddTheme(editTheme, "edit");
			serializer = new UI::ThemeSerializer(editTheme);
			app->GetWindow().SetVSync(0);

			Application::GetName().GetWindow().GetTitlebar().SetMenubarCallback([app]()
				{
					if (ImGui::BeginMenu("File"))
					{
						if (ImGui::MenuItem("Exit"))
						{
							app->Close();
						}
						ImGui::EndMenu();
					}

					if (ImGui::BeginMenu("Help"))
					{

						ImGui::EndMenu();
					}
				});
		}
		virtual void OnUpdate(float ts) override
		{
			Renderer::SetClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
			Renderer::Clear();
			
		}
		virtual void OnUIRender() override
		{


			if (show_demo_window)
				ImGui::ShowDemoWindow(&show_demo_window);

			// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
			{
				static float f = 0.0f;
				static int counter = 0;

				ImGui::Begin("Hello");                          // Create a window called "Hello, world!" and append into it.

				ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
				ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
				ImGui::Checkbox("Another Window", &show_another_window);
				ImGui::Checkbox("Image Window", &show_image);
				ImGui::Checkbox("Theme edit", &show_theme_edit);

				ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
				ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

				if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
					counter++;
				ImGui::SameLine();
				ImGui::Text("counter = %d", counter);

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
				ImGui::End();
			}

			// 3. Show another simple window.
			if (show_another_window)
			{
				ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
				ImGui::Text("Hello from another window!");
				if (ImGui::Button("Close Me"))
					show_another_window = false;
				ImGui::End();
			}
			if (show_image)
			{
			ImGui::Begin("Image Window");
			ImGui::Image((void*)image->GetData(), ImVec2{ 300, 300 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
			ImGui::End();
			}
			if (show_theme_edit)
			{
				ImGui::Begin("Theme Editor");
				if (ImGui::Button("Reset to default"))
					Application::GetName().GetWindow().GetSettings().SetTheme("default");
				ImGui::SameLine();
				if (ImGui::Button("Save"))
					serializer->Serialize();
				ImGui::SameLine();
				if (ImGui::Button("Load"))
					serializer->Deserialize();

				{
					auto& style = ImGui::GetStyle();
					auto& colors = ImGui::GetStyle().Colors;

					ImGui::NewLine();
					// Headers
					ImGui::ColorEdit4("Header", (float*)&colors[ImGuiCol_Header]);
					ImGui::ColorEdit4("HeaderoHovered", (float*)&colors[ImGuiCol_HeaderHovered]);
					ImGui::ColorEdit4("HeaderActive", (float*)&colors[ImGuiCol_HeaderActive]);
					//Buttons
					ImGui::ColorEdit4("Button", (float*)&colors[ImGuiCol_Button]);
					ImGui::ColorEdit4("ButtonHovered", (float*)&colors[ImGuiCol_ButtonHovered]);
					ImGui::ColorEdit4("ButtonActive", (float*)&colors[ImGuiCol_ButtonActive]);
					// Frame BG
					ImGui::ColorEdit4("FrameBg", (float*)&colors[ImGuiCol_FrameBg]);
					ImGui::ColorEdit4("FrameBgHovered", (float*)&colors[ImGuiCol_FrameBgHovered]);
					ImGui::ColorEdit4("FrameBgActive", (float*)&colors[ImGuiCol_FrameBgActive]);
					// Tabs
					ImGui::ColorEdit4("Tab", (float*)&colors[ImGuiCol_Tab]);
					ImGui::ColorEdit4("TabHovered", (float*)&colors[ImGuiCol_TabHovered]);
					ImGui::ColorEdit4("TabActive", (float*)&colors[ImGuiCol_TabActive]);
					ImGui::ColorEdit4("TabUnfocused", (float*)&colors[ImGuiCol_TabUnfocused]);
					ImGui::ColorEdit4("TabUnfocusedActive", (float*)&colors[ImGuiCol_TabUnfocusedActive]);
					// Title
					ImGui::ColorEdit4("TitleBg", (float*)&colors[ImGuiCol_TitleBg]);
					ImGui::ColorEdit4("TitleBgActive", (float*)&colors[ImGuiCol_TitleBgActive]);
					ImGui::ColorEdit4("TitleBgCollapsed", (float*)&colors[ImGuiCol_TitleBgCollapsed]);
					// Resize Grip
					ImGui::ColorEdit4("ResizeGrip", (float*)&colors[ImGuiCol_ResizeGrip]);
					ImGui::ColorEdit4("ResizeGripHovered", (float*)&colors[ImGuiCol_ResizeGripHovered]);
					ImGui::ColorEdit4("ResizeGripActive", (float*)&colors[ImGuiCol_ResizeGripActive]);
					// Scrollbar
					ImGui::ColorEdit4("ScrollbarBg", (float*)&colors[ImGuiCol_ScrollbarBg]);
					ImGui::ColorEdit4("scrollbarGrab", (float*)&colors[ImGuiCol_ScrollbarGrab]);
					ImGui::ColorEdit4("ScrollbarGrabHovered", (float*)&colors[ImGuiCol_ScrollbarGrabHovered]);
					ImGui::ColorEdit4("ScrollbarGrabActive", (float*)&colors[ImGuiCol_ScrollbarGrabActive]);
					// Check Mark
					ImGui::ColorEdit4("CheckMark", (float*)&colors[ImGuiCol_CheckMark]);
					// Slider
					ImGui::ColorEdit4("SliderGrab", (float*)&colors[ImGuiCol_SliderGrab]);
					ImGui::ColorEdit4("SliderGrabActive", (float*)&colors[ImGuiCol_SliderGrabActive]);
					// Text
					ImGui::ColorEdit4("Text", (float*)&colors[ImGuiCol_Text]);

					// Separator
					ImGui::ColorEdit4("Separator", (float*)&colors[ImGuiCol_Separator]);
					ImGui::ColorEdit4("SeparatorActive", (float*)&colors[ImGuiCol_SeparatorActive]);
					ImGui::ColorEdit4("SeparatorHovered", (float*)&colors[ImGuiCol_SeparatorHovered]);

					// Window Background
					ImGui::ColorEdit4("WindowBg", (float*)&colors[ImGuiCol_WindowBg]);
					ImGui::ColorEdit4("ChildBg", (float*)&colors[ImGuiCol_ChildBg]);
					ImGui::ColorEdit4("PopupBg", (float*)&colors[ImGuiCol_PopupBg]);
					ImGui::ColorEdit4("Border", (float*)&colors[ImGuiCol_Border]);
					// Tables
					ImGui::ColorEdit4("TableHeaderBg", (float*)&colors[ImGuiCol_TableHeaderBg]);
					ImGui::ColorEdit4("TableBorderLight", (float*)&colors[ImGuiCol_TableBorderLight]);

					// Menubar
					ImGui::ColorEdit4("MenuBarBg", (float*)&colors[ImGuiCol_MenuBarBg]);
					ImGui::SliderFloat("FrameRounding", (float*)&style.FrameRounding, 0.0f, 100.0f);
					ImGui::SliderFloat("FrameBorderSize", (float*)&style.FrameBorderSize, 0.0f, 100.0f);
					ImGui::SliderFloat("IndentSpacing", (float*)&style.IndentSpacing, 0.0f, 100.0f);
				}

				ImGui::End();

			}

		}

		virtual void OnEvent(Event& e) override {}
	private:
		bool show_demo_window = true;
		bool show_another_window = false;
		bool show_image = false;
		bool show_theme_edit = false;
		bool show_theme_def = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		ImGuiIO& io = ImGui::GetIO();
		std::unique_ptr<Image> image;
		float color[4];

		VieM::UI::Theme* editTheme;
		UI::ThemeSerializer* serializer;

	};
}