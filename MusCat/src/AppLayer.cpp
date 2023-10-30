#include "AppLayer.h"
#include <VieM/Core/Application.h>
#include <VieM/Render/Renderer.h>
#include <VieM/Core/Image.h>
#include <VieM/UI/UI.h>
#include <VieM/UI/Theme.h>
#include <VieM/Core/Log.h>

void AppLayer::OnAttach()
{
	using namespace std;
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
				if (ImGui::MenuItem("FPS", NULL, &m_FPSPanel.m_Enabled))
				{

				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Help"))
			{

				ImGui::EndMenu();
			}
		});





	m_View = make_shared<View>();
	m_Model = make_shared<Model>();
	m_Controller = make_shared<Controller>(m_Model, m_View);
	m_View->AddListener(m_Controller);
	m_Controller->Run();

}

void AppLayer::OnUpdate(float ts)
{

}



void AppLayer::OnUIRender()
{
	m_SettignsPanel.OnRender();
	m_FPSPanel.OnRender();

	// View
	m_View->OnRender();

}

