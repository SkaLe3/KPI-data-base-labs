#include "WDataSelection.h"
#include <imgui/imgui.h>
#include "MVC/Controller.h"

WDataSelection::WDataSelection()
{
	m_TableFlags = ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY;
	m_ColumnFlags = ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_NoClip;
}

void WDataSelection::OnRender()
{
	ImGui::Begin("Selections", NULL);

	if (ImGui::BeginTable("Table", 4, m_TableFlags))
	{

		ImGui::TableSetupScrollFreeze(0, 1); // Top Row always visible
		ImGui::TableSetupColumn("ID", m_ColumnFlags);
		ImGui::TableSetupColumn("Name", m_ColumnFlags);
		ImGui::TableSetupColumn("Date", m_ColumnFlags);
		ImGui::TableSetupColumn("Another", m_ColumnFlags);

		ImGui::TableHeadersRow();

		ImGuiListClipper clipper;
		clipper.Begin(100000);


		while (clipper.Step())
		{

			for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
			{
				ImGui::TableNextRow();
				for (int col = 0; col < 4; col++)
				{
					ImGui::TableSetColumnIndex(col);
					ImGui::Text("Data %d%d", row + 1, col + 1);
				}
			}
		}

		ImGui::EndTable();
	}
	
	ImGui::End();

	SelectionPanel();

}

void WDataSelection::SelectionPanel()
{
	ImGui::Begin("Selection Panel", NULL);

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10.0f, 10.0f));

	ImGui::Button("Select 1", ImVec2(300, 40));
	ImGui::Button("Select 2", ImVec2(300, 40));
	ImGui::Button("Select 3", ImVec2(300, 40));

	ImGui::Separator();

	ImGui::PushItemWidth(300);

	ImGui::PopItemWidth();




	ImVec2 buttonSize(300, 40);
	ImGui::SetCursorPos(ImVec2(ImGui::GetWindowContentRegionMin().x, ImGui::GetWindowContentRegionMax().y - buttonSize.y));
	if (ImGui::Button("< Go to Management", buttonSize))
		m_Callback->OnShowManagement();

	ImGui::PopStyleVar();

	ImGui::End();
}
