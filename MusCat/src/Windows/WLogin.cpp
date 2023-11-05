#include "WLogin.h"
#include <imgui/imgui.h>
#include "VieM/Core/Application.h"
#include "MVC/Controller.h"

void WLogin::OnRender()
{
	VieM::Window& window = VieM::Application::GetName().GetWindow();
	auto [x, y] = window.GetPos();
	int32_t width = window.GetWidth();
	int32_t height = window.GetHeight();

	ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(x + (width - 400) / 2, y + (height - 200) / 2), ImGuiCond_Always);

	ImGui::Begin("Admin Sign In", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

	ImGui::Dummy(ImVec2(0, 20));
	ImGui::Dummy(ImVec2(20, 0));
	ImGui::SameLine();

	ImGui::InputTextWithHint("Username", "postgres", m_Username, sizeof(m_Username));

	ImGui::Dummy(ImVec2{ 0, 5 });
	ImGui::Dummy(ImVec2(20, 0));
	ImGui::SameLine();

	ImGui::InputTextWithHint("Password", "****", m_Password, sizeof(m_Password), ImGuiInputTextFlags_Password);

	ImGui::Dummy(ImVec2{ 0, 5 });
	ImGui::Dummy(ImVec2{ 20, 0 });
	ImGui::SameLine();

	if (ImGui::Button("Log in", ImVec2{ 260, 30 }))
	{
		m_Callback->OnLogin(m_Username, m_Password);
	}


	ImGui::End();
}
