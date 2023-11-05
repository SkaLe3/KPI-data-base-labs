#include "Controller.h"
#include "View.h"
#include "Model.h"
#include "Windows/WLogin.h"
#include "Windows/WError.h"
#include "Windows/WManagementDB.h"
#include "Windows/WDataSelection.h"
#include "Windows/PopupWindows.h"
#include "Tables.h"
#include <algorithm>

//temp
#include "VieM/Core/Log.h"

using namespace std;


void Controller::OnUpdate()
{

}

void Controller::Run()
{
	CreateWindows();
	ShowWindow<WLogin>();

}

void Controller::OnLogin(const std::string& username, const std::string& password)
{
	HideWindow<WLogin>();

	bool success = m_Model->Connect(username, password, m_ErrorMessageText);
	if (!success)
	{
		ShowWindow<WError>();
		ShowWindow<WLogin>();
		return;
	}
	HideWindow<WError>();

	if (!m_Model->CreateTables(m_ErrorMessageText))
	{
		ShowWindow<WError>();
		return;
	}
	HideWindow<WError>();

	Table tabIndex = static_pointer_cast<WManagementDB>(m_View->GetWindow<WManagementDB>())->GetTabSelectionContext();
	OnSelectedTabChanged(tabIndex);

	ShowWindow<WManagementDB>();



}

void Controller::OnShowSelections()
{
	HideWindow<WManagementDB>();
	ShowWindow<WDataSelection>();
}

void Controller::OnShowManagement()
{
	HideWindow<WDataSelection>();
	ShowWindow<WManagementDB>();
}

void Controller::OnAddRecord(Table tableIndex)
{
	Table tabIndex = static_pointer_cast<WManagementDB>(m_View->GetWindow<WManagementDB>())->GetTabSelectionContext();
	static_pointer_cast<WAddRecord>(m_View->GetWindow<WAddRecord>())->SetSelectionContext(tabIndex);
	ShowWindow<WAddRecord>();
	HideWindow<WEditRecord>();
	HideWindow<WDeleteRecord>();
}

void Controller::OnEditRecord(Table tableIndex)
{
	Table tabIndex = static_pointer_cast<WManagementDB>(m_View->GetWindow<WManagementDB>())->GetTabSelectionContext();

	std::vector<std::string> pkeys = m_Model->GetPKeys(tabIndex, m_ErrorMessageText);
	if (!pkeys.empty())
	{

		std::vector<Column> newColumns;

		for (auto& column : TableSpecs::GetColumns(tabIndex))
		{
			for (auto& name : pkeys)
			{
				std::string nameCopy = column.Name;
				std::transform(nameCopy.begin(), nameCopy.end(), nameCopy.begin(),
					[](unsigned char c) { return std::tolower(c); }
				);

				if (nameCopy == name)
					newColumns.push_back(column);
			}
		}
		static_pointer_cast<WEditRecord>(m_View->GetWindow<WEditRecord>())->SetSelectionContext(tabIndex, newColumns);
	}
	else if (!m_ErrorMessageText.empty())
		ShowWindow<WError>();


	HideWindow<WAddRecord>();
	ShowWindow<WEditRecord>();
	HideWindow<WDeleteRecord>();
}

void Controller::OnDeleteRecord()
{
	HideWindow<WAddRecord>();
	HideWindow<WEditRecord>();
	ShowWindow<WDeleteRecord>();
}

void Controller::OnGenerateData()
{

}

void Controller::OnAddRecordSubmit(Table id,std::vector<std::string> data)
{
	bool success = m_Model->AddRecord(id, data, m_ErrorMessageText);
	if (!success)
		ShowWindow<WError>();
	else
	{
		OnSelectedTabChanged(id);
	}
}

void Controller::OnUpdateRecordSubmit(Table id, std::vector<std::string> data, std::vector<Column> keys, std::vector<std::string> keysData)
{
	bool success = m_Model->UpdateRecord(id, data, keys, keysData, m_ErrorMessageText);
	if (!success)
		ShowWindow<WError>();
	else
	{
		OnSelectedTabChanged(id);
	}
}

void Controller::OnSelectedTabChanged(Table tabIndex)
{
	std::vector<std::string> pkeys = m_Model->GetPKeys(tabIndex, m_ErrorMessageText);

	std::shared_ptr<TableData> data = m_Model->FetchTableData(tabIndex, pkeys, m_ErrorMessageText);
	if (data->empty() && !m_ErrorMessageText.empty())
		ShowWindow<WError>();
	else
		m_View->Update<WManagementDB>(data);
	HideWindow<WAddRecord>();
	HideWindow<WEditRecord>();
	HideWindow<WDeleteRecord>();
}

bool Controller::OnFindRecord(Table id, std::vector<Column> columns, std::vector<std::string> data)
{
	HideWindow<WError>();
	std::vector<std::string> recordData = m_Model->GetRecordIfExists(id,columns, data, m_ErrorMessageText);
	if (recordData.empty())
		ShowWindow<WError>();
	else
		m_View->Update<WEditRecord, std::vector<std::string>>(std::make_shared<std::vector<std::string>>(recordData));

	return !recordData.empty();
}

void Controller::CreateWindows()
{
	m_View->AddWindow<WLogin>();
	m_View->AddWindow<WError>();
	m_View->AddWindow<WManagementDB>();
	m_View->AddWindow<WDataSelection>();
	m_View->AddWindow<WAddRecord>();
	m_View->AddWindow<WEditRecord>();
	m_View->AddWindow<WDeleteRecord>();
}
