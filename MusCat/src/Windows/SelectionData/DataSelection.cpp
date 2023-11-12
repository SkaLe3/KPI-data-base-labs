#include "DataSelection.h"
#include <imgui/imgui.h>
#include "MVC/Controller.h"
#include "Utils/Inputs.h"

WDataSelection::WDataSelection()
{
	m_TableFlags = ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY;
	m_ColumnFlags = ImGuiTableColumnFlags_NoHide;
	m_TableData = std::make_shared<std::vector<std::vector<std::string>>>();
}

void WDataSelection::OnRender()
{
	TablePanel();

	SelectionPanel();
}

void WDataSelection::Update(std::shared_ptr<std::vector<std::vector<std::string>>> data)
{
	m_TableData = data;
}

void WDataSelection::SelectionPanel()
{
	ImGui::Begin("Selection Panel", NULL);

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10.0f, 10.0f));

	FirstSelection();
	SecondSelection();
	ThirdSelection();

	ImGui::Separator();




	ImVec2 buttonSize(300, 40);
	ImGui::SetCursorPos(ImVec2(ImGui::GetWindowContentRegionMin().x, ImGui::GetWindowContentRegionMax().y - buttonSize.y));
	if (ImGui::Button("< Go to Management", buttonSize))
		m_Callback->OnShowManagement();

	ImGui::PopStyleVar();

	ImGui::End();
}

void WDataSelection::TablePanel()
{
	ImGui::Begin("Selections", NULL);
	if (m_TableData->empty())
	{
		ImGui::End();
		return;
	}
	if (ImGui::BeginTable("Table", m_Columns.size(), m_TableFlags))
	{

		ImGui::TableSetupScrollFreeze(0, 1); // Top Row always visible

		for (std::string column : m_Columns)
		{
			ImGui::TableSetupColumn(column.c_str(), m_ColumnFlags);
		}

		ImGui::TableHeadersRow();

		ImGuiListClipper clipper;
		clipper.Begin(m_TableData->size());


		while (clipper.Step())
		{

			for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
			{
				ImGui::TableNextRow();
				for (int col = 0; col < m_Columns.size(); col++)
				{
					ImGui::TableSetColumnIndex(col);
					ImGui::Text("%s", (*m_TableData)[row][col].c_str());
				}
			}
		}

		ImGui::EndTable();
	}

	ImGui::End();
}

void WDataSelection::FirstSelection()
{
	ImGui::PushItemWidth(100);

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Text("Persons under"); ImGui::SameLine();
	ImGui::InputInt("##age", &m_Age, 1, 1); ImGui::SameLine();
	ImGui::Text("years");
	ImGui::Text("With more than"); ImGui::SameLine();
	ImGui::InputInt("##songs", &m_SongCount, 1, 1); ImGui::SameLine();
	ImGui::Text("songs");
	ImGui::Text("Of "); ImGui::SameLine();


	ImGui::PushItemWidth(180);
	std::string combo_preview_value = m_Genres[m_CurrentGenreIdx];

	if (ImGui::BeginCombo("##combo genre", combo_preview_value.c_str()))
	{
		for (int n = 0; n < m_Genres.size(); n++)
		{
			const bool is_selected = (m_CurrentGenreIdx == n);
			if (ImGui::Selectable(m_Genres[n].c_str(), is_selected))
				m_CurrentGenreIdx = n;

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();

	ImGui::SameLine();
	ImGui::Text("genre");
	ImGui::PopItemWidth();



	if (ImGui::Button("Select 1", ImVec2(300, 40)))
	{
		m_Callback->OnFirstSelection(m_Age, m_SongCount, m_CurrentGenreIdx + 1, m_Genres[m_CurrentGenreIdx]);
	}
	ImGui::Spacing(); ImGui::Separator();
}

void WDataSelection::SecondSelection()
{
	ImGui::PushItemWidth(100);

	ImGui::Spacing();


	ImGui::Text("The most pupular genres");
	ImGui::Text("By released songs amount");
	ImGui::Text("And the most popular artist");
	ImGui::Text("In interval from "); ImGui::SameLine();

	ImGui::PushItemWidth(180);

	Inputs::InputDate("##2from", m_FromDate2);
	ImGui::Text("To   \t\t\t\t\t"); ImGui::SameLine();
	Inputs::InputDate("##2to", m_ToDate2);

	ImGui::PopItemWidth();
	ImGui::PopItemWidth();



	if (ImGui::Button("Select 2", ImVec2(300, 40)))
	{
		m_Callback->OnSecondSelection(m_FromDate2, m_ToDate2);
	}
	ImGui::Spacing(); ImGui::Separator();
}

void WDataSelection::ThirdSelection()
{
	ImGui::PushItemWidth(180);

	ImGui::Spacing();

	ImGui::Text("Songs in interval");
	ImGui::Text("From "); ImGui::SameLine();
	Inputs::InputDate("##3from", m_FromDate3);
	ImGui::Text("To  \t"); ImGui::SameLine();
	Inputs::InputDate("##3to", m_ToDate3);

	ImGui::PopItemWidth();


	if (ImGui::Button("Select 3", ImVec2(300, 40)))
	{
		m_Callback->OnThirdSelection(m_FromDate3, m_ToDate3);
	}
	ImGui::Spacing(); ImGui::Separator();
}

