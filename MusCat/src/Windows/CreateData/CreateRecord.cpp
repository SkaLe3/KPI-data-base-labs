#include "CreateRecord.h"
#include "Windows/PopupWindows.h"
#include <VieM/Core/Application.h>
#include <imgui/imgui.h>
#include "MVC/Controller.h"
#include "misc/cpp/imgui_stdlib.h"
#include "Utils/Inputs.h"

void WCreateRecord::OnRender()
{
	int32_t currentWidth = CenterNextWindow(true, 400);

	ImGui::Begin("Add Record", &m_NotHidden, ImGuiWindowFlags_NoResize);

	DrawEditRecord(currentWidth);

	ImVec2 buttonSize(ImGui::GetContentRegionAvail().x, 40);
	ImGui::SetCursorPos(ImVec2(ImGui::GetWindowContentRegionMin().x, ImGui::GetWindowContentRegionMax().y - buttonSize.y));
	if (ImGui::Button("Submit", buttonSize))
	{
		m_Callback->OnCreateRecordSubmit(m_SelectionContext, m_TextBuffer);
	}


	ImGui::End();
}
