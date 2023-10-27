#include "View.h"
#include "Controller.h"
#include <VieM/Core/Application.h>
#include <VieM/Core/Window.h>
#include <VieM/Core/Log.h>
#include <imgui/imgui.h>


void View::OnRender()
{
	if (m_LoginWindowEnabled)
		LoginWindowRender();
	if (m_ErrorMessage)
		ErrorMessageRender();
	if (m_MainWindowEnabled)
		MainWindowRender();
}

void View::AddListener(const std::shared_ptr<Controller>& controller)
{
	m_Controller = controller; 
}

void View::LoginWindowRender()
{
	
	VieM::Window& window = VieM::Application::Get().GetWindow();
	auto [x, y] = window.GetPos();
	int32_t width = window.GetWidth();
	int32_t height = window.GetHeight();

	ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(x+(width-400)/2, y+(height-200)/2), ImGuiCond_Always);

	ImGui::Begin("Admin Sign In",&m_LoginWindowEnabled ,ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	
	ImGui::Dummy(ImVec2(0, 20));
	ImGui::Dummy(ImVec2(20, 0));
	ImGui::SameLine();

	ImGui::InputTextWithHint("Username","postgres", m_Username, sizeof(m_Username));

	ImGui::Dummy(ImVec2{ 0, 5 });
	ImGui::Dummy(ImVec2(20, 0));
	ImGui::SameLine();

	ImGui::InputTextWithHint("Password", "****", m_Password, sizeof(m_Password), ImGuiInputTextFlags_Password);
	
	ImGui::Dummy(ImVec2{ 0, 5 });
	ImGui::Dummy(ImVec2{ 20, 0 });
	ImGui::SameLine();

	if (ImGui::Button("Log in", ImVec2{260, 30}))
	{
		m_Controller->OnLogin(m_Username, m_Password);
	}


	ImGui::End();
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
	ImGui::TextWrapped(m_ErrorMessageText.c_str());
	ImGui::PopStyleColor();
	ImGui::End();



}

void View::MainWindowRender()
{
	ImGui::Begin("MusiCatalog");
	ImGui::Text("Some text hahahhaha");
	ImGui::End();
}
