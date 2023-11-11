#include "SampleDataStorage.h"
#include <fstream>

#include "VieM/Core/Log.h"

bool SampleDataStorage::LoadFiles()
{
	m_Names = LoadFile("assets/data/first_names.txt");
	m_Surnames = LoadFile("assets/data/last_names.txt");
	m_Genres = LoadFile("assets/data/genres.txt");
	m_Labels = LoadFile("assets/data/labels.txt");
	m_Locations = LoadFile("assets/data/locations.txt");
	m_Words = LoadFile("assets/data/words.txt");

	if (m_Names.empty() || m_Surnames.empty() || m_Genres.empty() || m_Labels.empty() || m_Locations.empty() || m_Words.empty())
		return false;
	return true;
}

std::vector<std::string> SampleDataStorage::LoadFile(const std::string& filename)
{
	std::vector<std::string> words;
	std::ifstream file(filename);

	if (!file.is_open())
	{
		VM_ERROR("Failed to open the file", filename);
		return words;
	}

	std::string word;

	while (std::getline(file, word))
	{
		words.push_back(word);
	}

	file.close();
	return words;
}

bool SampleDataStorage::CreateTables()
{
	bool success = true;
	success &= CreateTable(AuxiliaryTable::name, m_Names);
	success &= CreateTable(AuxiliaryTable::surname, m_Surnames);
	success &= CreateTable(AuxiliaryTable::words, m_Words);
	return success;
}

bool SampleDataStorage::CreateTable(AuxiliaryTable tableName, std::vector<std::string> data)
{
	bool tableExisted;
	std::string table = AuxiliaryTableSpecs::GetName(tableName);

	std::string checkTableSQL = "SELECT count(*) FROM information_schema.tables WHERE table_name = '" + table + "'";
	PGresult* checkRes = PQexec(m_Connection, checkTableSQL.c_str());

	if (PQresultStatus(checkRes) != PGRES_TUPLES_OK) {
		std::cerr << "Failed to check table existence: " << PQerrorMessage(m_Connection) << std::endl;
		PQclear(checkRes);
		PQfinish(m_Connection);
		return false;
	}

	int rowCount = atoi(PQgetvalue(checkRes, 0, 0));
	PQclear(checkRes);

	if (rowCount == 0) {
		tableExisted = false; // Table does not exist
	}
	else {
		tableExisted = true; // Table already exists
	}
	if (tableExisted)
		return true;

	std::string createTableSQL = "CREATE TABLE IF NOT EXISTS " + table; 
	createTableSQL+= " (" +
		AuxiliaryTableSpecs::GetColumns(tableName)[0].Name + " SERIAL PRIMARY KEY, " +
		AuxiliaryTableSpecs::GetColumns(tableName)[1].Name + " character varying(64))";

	PGresult* res = PQexec(m_Connection, createTableSQL.c_str());

	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		std::string errorMessage = "Table creation failed: " + std::string(PQerrorMessage(m_Connection));
		VM_ERROR(errorMessage);
		PQclear(res);
		PQfinish(m_Connection);

		return false;
	}

	PQclear(res);

	for (const std::string& record : data)
	{
		std::string insertSQL = "INSERT INTO " + table +
			+"("+ AuxiliaryTableSpecs::GetColumns(tableName)[1].Name +")VALUES($1)";

		const char* paramValues[1] = { record.c_str() };
		res = PQexecParams(m_Connection, insertSQL.c_str(), 1, NULL, paramValues, NULL, NULL, 0);

		if (PQresultStatus(res) != PGRES_COMMAND_OK)
		{
			std::string errorMessage = "Insertion failed for data '" + record +"' \n" + PQerrorMessage(m_Connection);
			VM_ERROR(errorMessage);
			PQclear(res);
			PQfinish(m_Connection);

			return false;
		}
		PQclear(res);
	}

	return true;
}

void SampleDataStorage::SetConnection(PGconn* connection)
{
	m_Connection = connection;
}


