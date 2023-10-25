#include "AppLayer.h"
#include <VieM/Core/Application.h>
#include <VieM/Render/Renderer.h>
#include <VieM/Core/Image.h>
#include <VieM/UI/UI.h>
#include <VieM/UI/Theme.h>


void AppLayer::OnAttach()
{
	VieM::Application* app = &VieM::Application::Get();
	app->GetWindow().GetTitlebar().SetMenubarCallback([app, this]()
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit"))
				{
					app->Close();
				}
				if (ImGui::MenuItem("Settings", NULL, &m_SettignsPanel.m_Enabled))
				{

				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Help"))
			{

				ImGui::EndMenu();
			}
		});
	app->GetWindow().SetVSync(0);
}

void AppLayer::OnUpdate(float ts)
{


}

void AppLayer::OnUIRender()
{
	m_SettignsPanel.OnRender();
	m_TilesPanel.OnRender();
}

