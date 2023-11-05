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
void WAddRecord::OnRender()
{
	VieM::Window& window = VieM::Application::GetName().GetWindow();
	auto [x, y] = window.GetPos();
	int32_t width = window.GetWidth();
	int32_t height = window.GetHeight();

	int32_t currentWidth = TableSpecs::GetColumns(m_SelectionContext).size() * 200;
	currentWidth = currentWidth > width ? width : currentWidth;

	ImGui::SetNextWindowSize(ImVec2(currentWidth, 200), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(x + (width - currentWidth) / 2, y + (height - 200) / 2), ImGuiCond_Appearing);

	ImGui::Begin("Add Record", &m_NotHidden, ImGuiWindowFlags_NoResize);
	std::vector<std::string> sendData;
	if (ImGui::BeginTable(TableSpecs::GetName(m_SelectionContext).c_str(), TableSpecs::GetColumns(m_SelectionContext).size(), m_TableFlags))
	{

		std::vector<Column> columns = TableSpecs::GetColumns(m_SelectionContext);
		for (uint32_t titleIdx = 0; titleIdx < columns.size(); titleIdx++)
			ImGui::TableSetupColumn(columns[titleIdx].Name.c_str(), m_ColumnFlags);

		ImGui::TableHeadersRow();

		
		for (int32_t i = 0, size = columns.size(); i < size; i++)
		{
			ImGui::TableNextColumn();
			ImGui::PushItemWidth(currentWidth / columns.size() - 30);
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

	ImVec2 buttonSize(ImGui::GetContentRegionAvail().x, 40);
	ImGui::SetCursorPos(ImVec2(ImGui::GetWindowContentRegionMin().x, ImGui::GetWindowContentRegionMax().y - buttonSize.y));
	if (ImGui::Button("Submit", buttonSize))
	{
		
		m_Callback->OnAddRecordSubmit(m_SelectionContext, m_TextBuffer);
	}


	ImGui::End();
}

// TODO:
// Add Text what to do when edit record window showed
void WEditRecord::OnRender()
{
	VieM::Window& window = VieM::Application::GetName().GetWindow();
	auto [x, y] = window.GetPos();
	int32_t width = window.GetWidth();
	int32_t height = window.GetHeight();

	int32_t currentWidth = TableSpecs::GetColumns(m_SelectionContext).size() * 200;
	currentWidth = currentWidth > width ? width : currentWidth;
	currentWidth = m_RowExist ? currentWidth : 400;

	ImGui::SetNextWindowSize(ImVec2(currentWidth, 200), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(x + (width - currentWidth) / 2, y + (height - 200) / 2), ImGuiCond_Appearing);

	ImGui::Begin("Edit Record", &m_NotHidden, ImGuiWindowFlags_NoResize);

	if (m_RowExist)
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
				ImGui::PushItemWidth(currentWidth / columns.size() - 30);
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
		if (ImGui::BeginTable(TableSpecs::GetName(m_SelectionContext).c_str(), m_Keys.size(), m_TableFlags))
		{

			for (uint32_t titleIdx = 0; titleIdx < m_Keys.size(); titleIdx++)
				ImGui::TableSetupColumn(m_Keys[titleIdx].Name.c_str(), m_ColumnFlags);

			ImGui::TableHeadersRow();

			for (int32_t i = 0, size = m_Keys.size(); i < size; i++)
			{
				ImGui::TableNextColumn();
				ImGui::PushItemWidth(currentWidth / m_Keys.size() - 30);
				if (m_Keys[i].Type == ColumnType::Serial)
				{
					if (ImGui::InputInt(("##" + m_Keys[i].Name).c_str(), &m_IntKeys[i]))
						m_TextKeys[i] = std::to_string(m_IntKeys[i]);
				}
				else if (m_Keys[i].Type == ColumnType::CharVar64)
				{
					if (ImGui::InputText(("##" + m_Keys[i].Name).c_str(), &m_TextKeys[i]))
						if (m_TextKeys[i].length() > 64)
							m_TextKeys[i] = m_TextKeys[i].substr(0, 64);
				}
				else if (m_Keys[i].Type == ColumnType::Text)
				{
					ImGui::InputText(("##" + m_Keys[i].Name).c_str(), &m_TextKeys[i]);
				}
				else if (m_Keys[i].Type == ColumnType::Date)
				{
					Inputs::InputDate(("##" + m_Keys[i].Name).c_str(), m_TextKeys[i]);

				}
				else if (m_Keys[i].Type == ColumnType::Int)
				{
					if (ImGui::InputInt(("##" + m_Keys[i].Name).c_str(), &m_IntKeys[i]))
						m_TextKeys[i] = std::to_string(m_IntKeys[i]);
				}
				ImGui::PopItemWidth();
			}
			ImGui::EndTable();
		}
		ImVec2 buttonSize(ImGui::GetContentRegionAvail().x, 40);
		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowContentRegionMin().x, ImGui::GetWindowContentRegionMax().y - buttonSize.y));
		if (ImGui::Button("Find Record", buttonSize))
		{

			m_RowExist = m_Callback->OnFindRecord(m_SelectionContext, m_Keys,m_TextKeys);
			m_KeysData = m_TextKeys;
			
		}
	}
	ImGui::End();
}

void WEditRecord::Update(std::shared_ptr<std::vector<std::string>> data)
{

	m_TextBuffer = *data;

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

void WEditRecord::SetSelectionContext(Table id, const std::vector<Column>& keys)
{

	PopupTableManagement::SetSelectionContext(id);
	m_Keys = keys;

	m_TextKeys.clear();
	m_IntKeys.clear();

	m_TextKeys.resize(m_Keys.size(), "");

	m_IntKeys.resize(m_Keys.size(), 0);

	m_RowExist = false;

}

void WDeleteRecord::OnRender()
{
	VieM::Window& window = VieM::Application::GetName().GetWindow();
	auto [x, y] = window.GetPos();
	int32_t width = window.GetWidth();
	int32_t height = window.GetHeight();

	ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(x + (width - 400) / 2, y + (height - 200) / 2), ImGuiCond_Appearing);

	ImGui::Begin("Delete Record", &m_NotHidden, ImGuiWindowFlags_NoResize);

	ImGui::End();
}

