#include "PopupWindows.h"
#include <VieM/Core/Application.h>
#include <imgui/imgui.h>
#include "MVC/Controller.h"
#include "misc/cpp/imgui_stdlib.h"
#include "Utils/Inputs.h"

PopupTableManagement::PopupTableManagement()
{
	m_TableFlags = ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders;
	m_ColumnFlags = ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_NoClip;
}

void PopupTableManagement::SetSelectionContext(Table id)
{
	m_SelectionContext = id;

	m_TextBuffer.clear();
	m_IntBuffer.clear();

	std::vector<Column>columns = TableSpecs::GetColumns(id);
	m_TextBuffer.resize(columns.size(), "");

	m_IntBuffer.resize(columns.size(), 0);


}

int32_t PopupTableManagement::CenterNextWindow(bool check, int32_t newWidth)
{
	VieM::Window& window = VieM::Application::GetName().GetWindow();
	auto [x, y] = window.GetPos();
	int32_t width = window.GetWidth();
	int32_t height = window.GetHeight();

	int32_t currentWidth = TableSpecs::GetColumns(m_SelectionContext).size() * 200;
	currentWidth = currentWidth > width ? width : currentWidth;
	currentWidth = check ? currentWidth : newWidth;

	ImGui::SetNextWindowSize(ImVec2(currentWidth, 200), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(x + (width - currentWidth) / 2, y + (height - 200) / 2), ImGuiCond_Appearing);
	return currentWidth;
}

void PopupTableManagement::DrawEditRecord(int32_t width)
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
			ImGui::PushItemWidth(width / columns.size() - 30);
			if (columns[i].Type == ColumnType::Serial)
				ImGui::Text("Default");
			else if (columns[i].Type == ColumnType::CharVar64)
			{
				if (ImGui::InputText(("##" + columns[i].Name).c_str(), &m_TextBuffer[i]))
					if (m_TextBuffer[i].length() > 64)
						m_TextBuffer[i] = m_TextBuffer[i].substr(0, 64);
			}
			else if (columns[i].Type == ColumnType::Text)
			{
				ImGui::InputText(("##" + columns[i].Name).c_str(), &m_TextBuffer[i]);
			}
			else if (columns[i].Type == ColumnType::Date)
			{
				Inputs::InputDate(("##" + columns[i].Name).c_str(), m_TextBuffer[i]);

			}
			else if (columns[i].Type == ColumnType::Int)
			{
				if (ImGui::InputInt(("##" + columns[i].Name).c_str(), &m_IntBuffer[i]))
					m_TextBuffer[i] = std::to_string(m_IntBuffer[i]);
			}


			ImGui::PopItemWidth();

		}
		ImGui::EndTable();
	}
}

void PopupTableManagement::DrawFindRecord(int32_t width, std::vector<Column> keyColumns, std::vector<std::string>& textKeysData, std::vector<int32_t>& intKeysData)
{
	if (ImGui::BeginTable(TableSpecs::GetName(m_SelectionContext).c_str(), keyColumns.size(), m_TableFlags))
	{

		for (uint32_t titleIdx = 0; titleIdx < keyColumns.size(); titleIdx++)
			ImGui::TableSetupColumn(keyColumns[titleIdx].Name.c_str(), m_ColumnFlags);

		ImGui::TableHeadersRow();

		for (int32_t i = 0, size = keyColumns.size(); i < size; i++)
		{
			ImGui::TableNextColumn();
			ImGui::PushItemWidth(width / keyColumns.size() - 30);
			if (keyColumns[i].Type == ColumnType::Serial)
			{
				if (ImGui::InputInt(("##" + keyColumns[i].Name).c_str(), &intKeysData[i]))
					textKeysData[i] = std::to_string(intKeysData[i]);
			}
			else if (keyColumns[i].Type == ColumnType::CharVar64)
			{
				if (ImGui::InputText(("##" + keyColumns[i].Name).c_str(), &textKeysData[i]))
					if (textKeysData[i].length() > 64)
						textKeysData[i] = textKeysData[i].substr(0, 64);
			}
			else if (keyColumns[i].Type == ColumnType::Text)
			{
				ImGui::InputText(("##" + keyColumns[i].Name).c_str(), &textKeysData[i]);
			}
			else if (keyColumns[i].Type == ColumnType::Date)
			{
				Inputs::InputDate(("##" + keyColumns[i].Name).c_str(), textKeysData[i]);

			}
			else if (keyColumns[i].Type == ColumnType::Int)
			{
				if (ImGui::InputInt(("##" + keyColumns[i].Name).c_str(), &intKeysData[i]))
					textKeysData[i] = std::to_string(intKeysData[i]);
			}
			ImGui::PopItemWidth();
		}
		ImGui::EndTable();
	}
}


