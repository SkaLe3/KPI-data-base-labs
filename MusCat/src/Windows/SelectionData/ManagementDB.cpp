#include "ManagementDB.h"
#include <imgui/imgui.h>
#include "MVC/Controller.h"
// temp
#include <VieM/Core/Log.h>

WManagementDB::WManagementDB()
{
	m_TableFlags = ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY;
	m_ColumnFlags = ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_NoClip;

}

void WManagementDB::OnRender()
{
	ImGui::Begin("Catalog Data", NULL);
	if (ImGui::BeginTabBar("Tabs"))
	{
		DrawTab(Table::Genre);
		DrawTab(Table::Label);
		DrawTab(Table::Album);
		DrawTab(Table::Song);
		DrawTab(Table::Person);
		DrawTab(Table::Artist);
		DrawTab(Table::Artist_Person);
		DrawTab(Table::Artist_Song);

		ImGui::EndTabBar();
	}
	ImGui::End();

	ControlPanel();
}

void WManagementDB::ControlPanel()
{
	ImGui::Begin("Control Panel", NULL);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10.0f, 10.0f));

	if (ImGui::Button("Create Record", ImVec2(300, 40)))
		m_Callback->OnCreateRecord(m_TabSelectionContext);

	if (ImGui::Button("Update Record", ImVec2(300, 40)))
		m_Callback->OnUpdateRecord(m_TabSelectionContext);

	if (ImGui::Button("Delete Record", ImVec2(300, 40)))
		m_Callback->OnDeleteRecord(m_TabSelectionContext);

	ImGui::Separator();

	ImGui::PushItemWidth(300);
	if (ImGui::InputInt("##", &m_GenerateRowsCount, 1, 100))
		if (m_GenerateRowsCount < 0)
			m_GenerateRowsCount = 0;
	ImGui::PopItemWidth();

	if (ImGui::Button("Generate Data", ImVec2(300, 40)))
		m_GenerateTime = m_Callback->OnGenerateData(m_TabSelectionContext, m_GenerateRowsCount);

	if (m_GenerateTime != 0)
		ImGui::Text("Last data generation time: %.0lf (ms)", m_GenerateTime * 1000);

	ImVec2 buttonSize(300, 40);
	ImGui::SetCursorPos(ImVec2(ImGui::GetWindowContentRegionMin().x, ImGui::GetWindowContentRegionMax().y - buttonSize.y));
	if (ImGui::Button("< Go to Selections", buttonSize))
		m_Callback->OnShowSelections();

	ImGui::PopStyleVar();

	ImGui::End();
}

void WManagementDB::DrawTab(Table id)
{
	if (ImGui::BeginTabItem(TableSpecs::GetName(id).c_str()))
	{
		m_NewTabSelectionContext = id;

		if (m_TabSelectionContext != m_NewTabSelectionContext)
		{
			m_Callback->OnFetchData(m_NewTabSelectionContext);
		}
		m_TabSelectionContext = m_NewTabSelectionContext;

		if (ImGui::BeginTable(TableSpecs::GetName(id).c_str(), TableSpecs::GetColumns(id).size(), m_TableFlags))
		{

			ImGui::TableSetupScrollFreeze(0, 1); // Top Row always visible
			
			std::vector<Column> columns = TableSpecs::GetColumns(id);
			for (uint32_t titleIdx = 0; titleIdx < columns.size(); titleIdx++)
				ImGui::TableSetupColumn(columns[titleIdx].Name.c_str(), m_ColumnFlags);

			ImGui::TableHeadersRow();

			ImGuiListClipper clipper;
			clipper.Begin(m_TableData->size());

			while (clipper.Step())
			{
				for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
				{
					ImGui::TableNextRow();
					for (int col = 0, size = TableSpecs::GetColumns(id).size(); col < size ; col++)
					{
						ImGui::TableSetColumnIndex(col);
						ImGui::Text("%s", (*m_TableData)[row][col].c_str());
					}
				}
			}
			ImGui::EndTable();
		}
		ImGui::EndTabItem();
	}
}
