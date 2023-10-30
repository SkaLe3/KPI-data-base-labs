#include "View.h"
#include "Controller.h"
#include <VieM/Core/Application.h>
#include <VieM/Core/Window.h>
#include <VieM/Core/Log.h>
#include <imgui/imgui.h>


void View::OnRender()
{
	if (m_ErrorMessage)
		ErrorMessageRender();
	if (m_MainPanelsEnabled)
	{
		MainWindowRender();
		ControlPanelRender();
	}

	for (std::shared_ptr<IWindowUI> window : m_Windows)
	{
		if (window->IsNotHidden())
			window->OnRender();
	}

}

void View::AddListener(const std::shared_ptr<Controller>& controller)
{
	m_Controller = controller; 
}

void View::RemoveAllWindows()
{
	m_Windows.clear();
}



void View::ErrorMessageRender()
{
	VieM::Window& window = VieM::Application::Get().GetWindow();
	auto [x, y] = window.GetPos();
	int32_t width = window.GetWidth();
	int32_t height = window.GetHeight();

	ImGui::SetNextWindowSize(ImVec2(280, 120), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(x + width - 265, y+34), ImGuiCond_Always);

	ImGui::Begin("Error", &m_ErrorMessage, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	//ImGui::TextColored(ImVec4{ 0.8f, 0.2f, 0.2f, 1.0f }, m_ErrorMessageText.c_str());
	ImGui::PushStyleColor(ImGuiCol_Text, {0.8f, 0.2f, 0.2f, 1.0f});
	ImGui::TextWrapped(m_Controller->GetErrorMessage().c_str());
	ImGui::PopStyleColor();
	ImGui::End();



}

void View::MainWindowRender()
{
	ImGui::Begin("MusiCatalog");
	ImGui::Text("Main Window");
	ImGui::End();
}

void View::ControlPanelRender()
{
	ImGui::Begin("MusiCatalog");
	
	ImGui::End();
}
