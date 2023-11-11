#include "UpdateRecord.h"
#include <VieM/Core/Application.h>
#include <imgui/imgui.h>
#include "MVC/Controller.h"
#include "misc/cpp/imgui_stdlib.h"
#include "Utils/Inputs.h"

void WUpdateRecord::OnRender()
{
	int32_t currentWidth = CenterNextWindow(m_RecordExist, 400);

	ImGui::Begin("Update Record", &m_NotHidden, ImGuiWindowFlags_NoResize);

	if (m_RecordExist)
	{
		DrawEditRecord(currentWidth);

		ImGui::TextColored({ 0.5f, 0.7f, 0.5f, 1.0f }, "Record Found");

		ImVec2 buttonSize(ImGui::GetContentRegionAvail().x, 40);
		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowContentRegionMin().x, ImGui::GetWindowContentRegionMax().y - buttonSize.y));

		if (ImGui::Button("Submit", buttonSize))
		{
			m_Callback->OnUpdateRecordSubmit(m_SelectionContext, m_TextBuffer, m_Keys, m_KeysData);
		}

	}
	else
	{
		ImGui::Text("\t\t\t\t\t\tFind Record To Update");
		
		DrawFindRecord(currentWidth, m_Keys, m_TextKeys, m_IntKeys);

		ImVec2 buttonSize(ImGui::GetContentRegionAvail().x, 40);
		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowContentRegionMin().x, ImGui::GetWindowContentRegionMax().y - buttonSize.y));
		if (ImGui::Button("Find Record", buttonSize))
		{

			m_RecordExist = m_Callback->OnFindRecord(m_SelectionContext, m_Keys, m_TextKeys, this);
			m_KeysData = m_TextKeys;

		}
	}
	ImGui::End();
}

void WUpdateRecord::Update(std::vector<std::string> data)
{

	m_TextBuffer = data;

	auto IsNumber = [](const std::string& inputString)
	{
		bool isnumber = true;
		for (char c : inputString)
		{
			if (!std::isdigit(c))
				isnumber = false;
		}
		return isnumber && !inputString.empty();
	};

	for (int32_t i = 0; i < m_TextBuffer.size(); i++)
	{
		if (IsNumber(m_TextBuffer[i]))
			m_IntBuffer[i] = std::stoi(m_TextBuffer[i]);
	}
}

void WUpdateRecord::SetSelectionContext(Table id, const std::vector<Column>& keys)
{

	PopupTableManagement::SetSelectionContext(id);
	m_Keys = keys;

	m_TextKeys.clear();
	m_IntKeys.clear();

	m_TextKeys.resize(m_Keys.size(), "");

	m_IntKeys.resize(m_Keys.size(), 0);

	m_RecordExist = false;

}
