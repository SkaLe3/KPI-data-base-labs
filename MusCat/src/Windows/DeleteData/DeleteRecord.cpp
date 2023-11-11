#include "DeleteRecord.h"
#include "Windows/PopupWindows.h"
#include <VieM/Core/Application.h>
#include <imgui/imgui.h>
#include "MVC/Controller.h"
#include "misc/cpp/imgui_stdlib.h"
#include "Utils/Inputs.h"


void WDeleteRecord::OnRender()
{
	int32_t currentWidth = CenterNextWindow(m_RecordExist, 400);

	ImGui::Begin("Delete Record", &m_NotHidden, ImGuiWindowFlags_NoResize);

	if (m_RecordExist)
	{
		DrawRecordData();

		ImGui::TextColored({ 0.5f, 0.7f, 0.5f, 1.0f }, "Record Found");
		ImVec2 buttonSize(ImGui::GetContentRegionAvail().x, 40);
		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowContentRegionMin().x, ImGui::GetWindowContentRegionMax().y - buttonSize.y));
		if (ImGui::Button("DELETE", buttonSize))
		{

			m_Callback->OnDeleteRecordSubmit(m_SelectionContext, m_Keys, m_TextKeys);
		}
	}
	else
	{
		ImGui::Text("\t\t\t\t\t\tFind Record To Delete");

		DrawFindRecord(currentWidth, m_Keys, m_TextKeys, m_IntKeys);

		ImVec2 buttonSize(ImGui::GetContentRegionAvail().x, 40);
		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowContentRegionMin().x, ImGui::GetWindowContentRegionMax().y - buttonSize.y));
		if (ImGui::Button("Find Record", buttonSize))
		{

			m_RecordExist = m_Callback->OnFindRecord(m_SelectionContext, m_Keys, m_TextKeys, this);

		}
	}
	ImGui::End();
}

void WDeleteRecord::SetSelectionContext(Table id, const std::vector<Column>& keys)
{

	PopupTableManagement::SetSelectionContext(id);
	m_Keys = keys;

	m_TextKeys.clear();
	m_IntKeys.clear();

	m_TextKeys.resize(m_Keys.size(), "");

	m_IntKeys.resize(m_Keys.size(), 0);

	m_RecordExist = false;

}

void WDeleteRecord::DrawRecordData()
{
	if (ImGui::BeginTable(TableSpecs::GetName(m_SelectionContext).c_str(), TableSpecs::GetColumns(m_SelectionContext).size(), m_TableFlags))
	{

		std::vector<Column> columns = TableSpecs::GetColumns(m_SelectionContext);
		for (uint32_t titleIdx = 0; titleIdx < columns.size(); titleIdx++)
			ImGui::TableSetupColumn(columns[titleIdx].Name.c_str(), m_ColumnFlags);

		ImGui::TableHeadersRow();


		for (int32_t i = 0, size = columns.size(); i < size; i++)
		{
			ImGui::TableNextColumn();
			ImGui::Text(m_TextBuffer[i].c_str());

		}
		ImGui::EndTable();
	}
}

void WDeleteRecord::Update(std::vector<std::string> data)
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