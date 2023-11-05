#include "SettingsPanel.h"
#include <VieM/Core/Application.h>
#include <imgui.h>

// temp
#include "VieM/Core/Image.h"
#include <string>
#include <chrono>
#include <string>
#include <thread>

void SettingsPanel::OnRender()
{
	

	if (!m_Enabled)
		return;

	ImGui::Begin("Settings");
	// Add function
	static int themeIdx;
	if (ImGui::Combo("Themes", &themeIdx, "Default Walnut\0Default Dark\0Default Light\0Default Classic\0"))
	{
		switch (themeIdx)
		{
		case 0:
		{
			auto& settings = VieM::Application::GetName().GetWindow().GetSettings();
			settings.SetTheme("default");
			break;
		}
		case 1: ImGui::StyleColorsDark(); break;
		case 2: ImGui::StyleColorsLight(); break;
		case 3: ImGui::StyleColorsClassic(); break;
		}
	}
	///
	

	ImGui::End();

	
}

