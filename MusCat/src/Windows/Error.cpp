#include "Error.h"
#include <VieM/Core/Application.h>
#include <imgui/imgui.h>
#include "MVC/Controller.h"

void WError::OnRender()
{
	VieM::Window& window = VieM::Application::GetName().GetWindow();
	auto [x, y] = window.GetPos();
	int32_t width = window.GetWidth();
	int32_t height = window.GetHeight();

	ImGui::SetNextWindowSize(ImVec2(330, 120), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(x + width - 315, y + 34), ImGuiCond_Always);

	ImGui::Begin("Error", &m_NotHidden, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	//ImGui::TextColored(ImVec4{ 0.8f, 0.2f, 0.2f, 1.0f }, m_ErrorMessageText.c_str());
	ImGui::PushStyleColor(ImGuiCol_Text, { 0.8f, 0.2f, 0.2f, 1.0f });
	ImGui::TextWrapped(m_Callback->GetErrorMessage().c_str());
	ImGui::PopStyleColor();
	ImGui::End();


}
