#include "Controller.h"
#include "View.h"
#include "Model.h"
#include "Windows/Login.h"
#include "Windows/Error.h"
#include "Windows/SelectionData/ManagementDB.h"
#include "Windows/SelectionData/DataSelection.h"
#include "Windows/PopupWindows.h"
#include "Windows/CreateData/CreateRecord.h"
#include "Windows/UpdateData/UpdateRecord.h"
#include "Windows/DeleteData/DeleteRecord.h"
#include "Tables.h"
#include <algorithm>
#include <chrono>

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

void Controller::OnLogin(const std::string& dbname, const std::string& username, const std::string& password)
{
	HideWindow<WLogin>();

	bool success = m_Model->Connect(dbname, username, password, m_ErrorMessageText);
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

	if (!m_Model->LoadTestDataSamples(m_ErrorMessageText))
	{
		ShowWindow<WError>();
		return;
	}
	if (!m_Model->CreateAuxiliaryTablesForTestData(m_ErrorMessageText))
	{
		ShowWindow<WError>();
		return;
	}

	Table tabIndex = static_pointer_cast<WManagementDB>(m_View->GetWindow<WManagementDB>())->GetTabSelectionContext();
	OnFetchData(tabIndex);

	ShowWindow<WManagementDB>();



}

void Controller::OnShowSelections()
{
	std::vector<std::string> genres =  m_Model->GetListOfGenres(m_ErrorMessageText);
	static_pointer_cast<WDataSelection>(m_View->GetWindow<WDataSelection>())->Setup(genres);

	HideWindow<WManagementDB>();
	ShowWindow<WDataSelection>();
}

void Controller::OnShowManagement()
{
	HideWindow<WDataSelection>();
	ShowWindow<WManagementDB>();
}

void Controller::OnCreateRecord(Table id)
{
	static_pointer_cast<WCreateRecord>(m_View->GetWindow<WCreateRecord>())->SetSelectionContext(id);

	ShowWindow<WCreateRecord>();
	HideWindow<WUpdateRecord>();
	HideWindow<WDeleteRecord>();
}

void Controller::OnUpdateRecord(Table id)
{
	std::vector<std::string> pkeysTitles = m_Model->GetPKeyColumnTitles(id, m_ErrorMessageText);

	if (!pkeysTitles.empty())
	{
		std::vector<Column> pkeyColumns;

		for (auto& column : TableSpecs::GetColumns(id))
		{
			for (auto& pkeyTitle : pkeysTitles)
			{
				std::string columnNameCopy = column.Name;
				std::transform(columnNameCopy.begin(), columnNameCopy.end(), columnNameCopy.begin(),
					[](unsigned char c) { return std::tolower(c); }
				);

				if (columnNameCopy == pkeyTitle)
					pkeyColumns.push_back(column);
			}
		}
		static_pointer_cast<WUpdateRecord>(m_View->GetWindow<WUpdateRecord>())->SetSelectionContext(id, pkeyColumns);
	}
	else if (!m_ErrorMessageText.empty())
		ShowWindow<WError>();


	HideWindow<WCreateRecord>();
	ShowWindow<WUpdateRecord>();
	HideWindow<WDeleteRecord>();
}

void Controller::OnDeleteRecord(Table id)
{
	std::vector<std::string> pkeysTitles = m_Model->GetPKeyColumnTitles(id, m_ErrorMessageText);

	if (!pkeysTitles.empty())
	{
		std::vector<Column> pkeyColumns;

		for (auto& column : TableSpecs::GetColumns(id))
		{
			for (auto& pkeyTitle : pkeysTitles)
			{
				std::string columnNameCopy = column.Name;
				std::transform(columnNameCopy.begin(), columnNameCopy.end(), columnNameCopy.begin(),
					[](unsigned char c) { return std::tolower(c); }
				);

				if (columnNameCopy == pkeyTitle)
					pkeyColumns.push_back(column);
			}
		}
		static_pointer_cast<WDeleteRecord>(m_View->GetWindow<WDeleteRecord>())->SetSelectionContext(id, pkeyColumns);
	}
	else if (!m_ErrorMessageText.empty())
		ShowWindow<WError>();

	HideWindow<WCreateRecord>();
	HideWindow<WUpdateRecord>();
	ShowWindow<WDeleteRecord>();
}

double Controller::OnGenerateData(Table id, int32_t rowCount)
{
	auto startTime = std::chrono::high_resolution_clock::now();

	std::chrono::steady_clock::time_point endTime;
	double duration = 0;

	if (!m_Model->GenerateData(id,rowCount, m_ErrorMessageText))
	{
		ShowWindow<WError>();
	}
	else
	{
		endTime = std::chrono::high_resolution_clock::now();
		duration = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime).count();
		OnFetchData(id);
	}

	return duration;
}

void Controller::OnCreateRecordSubmit(Table id,std::vector<std::string> recordData)
{
	bool success = m_Model->CreateRecord(id, recordData, m_ErrorMessageText);
	if (!success)
		ShowWindow<WError>();
	else
	{
		OnFetchData(id);
	}
}

void Controller::OnUpdateRecordSubmit(Table id, std::vector<std::string> recordData, std::vector<Column> pkeyColumns, std::vector<std::string> pkeysData)
{
	bool success = m_Model->UpdateRecord(id, recordData, pkeyColumns, pkeysData, m_ErrorMessageText);
	if (!success)
		ShowWindow<WError>();
	else
	{
		OnFetchData(id);
	}
}

void Controller::OnDeleteRecordSubmit(Table id, std::vector<Column> pkeyColumns, std::vector<std::string> pkeysData)
{
	bool success = m_Model->DeleteRecord(id, pkeyColumns, pkeysData, m_ErrorMessageText);
	if (!success)
		ShowWindow<WError>();
	else
	{
		OnFetchData(id);
	}
}

void Controller::OnFetchData(Table id)
{
	std::vector<std::string> pkeysTitles = m_Model->GetPKeyColumnTitles(id, m_ErrorMessageText);

	std::shared_ptr<TableData> tableData = m_Model->FetchTableData(id, pkeysTitles, m_ErrorMessageText);
	if (tableData->empty() && !m_ErrorMessageText.empty())
		ShowWindow<WError>();
	else
		m_View->Update<WManagementDB>(tableData);

	HideWindow<WCreateRecord>();
	HideWindow<WUpdateRecord>();
	HideWindow<WDeleteRecord>();
}

bool Controller::OnFindRecord(Table id, std::vector<Column> pkeyColumns, std::vector<std::string> pkeysData, IWindowUI* sender)
{
	HideWindow<WError>();
	std::vector<std::string> recordData = m_Model->GetRecordIfExists(id, pkeyColumns, pkeysData, m_ErrorMessageText);
	if (recordData.empty())
		ShowWindow<WError>();
	else
		sender->Update(recordData);

	return !recordData.empty();
}

double Controller::OnFirstSelection(int32_t age, int32_t songCount,int32_t genreId, std::string genre)
{
	auto startTime = std::chrono::high_resolution_clock::now();

	auto [data, columns] = m_Model->ExecuteFirstSelection(age, songCount,genreId, genre, m_ErrorMessageText);
	
	std::chrono::steady_clock::time_point endTime;
	double duration = 0;

	if (data->empty() && !m_ErrorMessageText.empty())
		ShowWindow<WError>();
	else
	{
		m_View->Update<WDataSelection>(data);
		static_pointer_cast<WDataSelection>(m_View->GetWindow<WDataSelection>())->SetColumns(columns);
		endTime = std::chrono::high_resolution_clock::now();
		duration = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime).count();
	}
	return duration;
}

double Controller::OnSecondSelection(const std::string& fromDate, const std::string& toDate)
{
	auto startTime = std::chrono::high_resolution_clock::now();

	auto [data, columns] = m_Model->ExecuteSecondSelection(fromDate, toDate, m_ErrorMessageText);

	std::chrono::steady_clock::time_point endTime;
	double duration = 0;

	if (data->empty() && !m_ErrorMessageText.empty())
		ShowWindow<WError>();
	else
	{
		m_View->Update<WDataSelection>(data);
		static_pointer_cast<WDataSelection>(m_View->GetWindow<WDataSelection>())->SetColumns(columns);
		endTime = std::chrono::high_resolution_clock::now();
		duration = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime).count();
	}
	return duration;
}

double Controller::OnThirdSelection(const std::string& fromDate, const std::string& toDate)
{
	auto startTime = std::chrono::high_resolution_clock::now();

	auto [data, columns] = m_Model->ExecuteThirdSelection(fromDate, toDate, m_ErrorMessageText);

	std::chrono::steady_clock::time_point endTime;
	double duration = 0;

	if (data->empty() && !m_ErrorMessageText.empty())
		ShowWindow<WError>();
	else
	{
		m_View->Update<WDataSelection>(data);
		static_pointer_cast<WDataSelection>(m_View->GetWindow<WDataSelection>())->SetColumns(columns);
		endTime = std::chrono::high_resolution_clock::now();
		duration = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime).count();
	}
	return duration;
}

void Controller::CreateWindows()
{
	m_View->AddWindow<WLogin>();
	m_View->AddWindow<WError>();
	m_View->AddWindow<WManagementDB>();
	m_View->AddWindow<WDataSelection>();
	m_View->AddWindow<WCreateRecord>();
	m_View->AddWindow<WUpdateRecord>();
	m_View->AddWindow<WDeleteRecord>();
}
