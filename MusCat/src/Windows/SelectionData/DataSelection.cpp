#include "DataSelection.h"
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
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10.0f, 10.0f));
	ImGui::PushItemWidth(100);
	ImGui::Text("Persons under"); ImGui::SameLine();
	static int age =0;
	ImGui::InputInt("##age", &age, 1, 1); ImGui::SameLine();
	ImGui::Text("years");
	ImGui::Text("With more than"); ImGui::SameLine();
	static int songs=0;
	ImGui::InputInt("##songs", &songs, 1, 1); ImGui::SameLine();
	ImGui::Text("songs");
	ImGui::Text("Of"); ImGui::SameLine();


	const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
	static int item_current_idx = 0; // Here we store our selection data as an index.
	const char* combo_preview_value = items[item_current_idx];  // Pass in the preview value visible before opening the combo (it could be anything)

	if (ImGui::BeginCombo("##combo genre", combo_preview_value))
	{
		for (int n = 0; n < IM_ARRAYSIZE(items); n++)
		{
			const bool is_selected = (item_current_idx == n);
			if (ImGui::Selectable(items[n], is_selected))
				item_current_idx = n;

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	ImGui::SameLine();
	ImGui::Text("genre");
	ImGui::PopItemWidth();
	ImGui::PushItemWidth(300);
	ImGui::Button("Select", ImVec2(300, 40));
	ImGui::Spacing(); ImGui::Separator();


	ImGui::Button("Select 2", ImVec2(300, 40));
	ImGui::Button("Select 3", ImVec2(300, 40));

	ImGui::Separator();


	
	ImGui::PopItemWidth();



	ImVec2 buttonSize(300, 40);
	ImGui::SetCursorPos(ImVec2(ImGui::GetWindowContentRegionMin().x, ImGui::GetWindowContentRegionMax().y - buttonSize.y));
	if (ImGui::Button("< Go to Management", buttonSize))
		m_Callback->OnShowManagement();

	ImGui::PopStyleVar();

	ImGui::End();
}

