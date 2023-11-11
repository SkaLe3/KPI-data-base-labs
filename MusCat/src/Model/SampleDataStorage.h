#pragma once
#include <vector>
#include <string>
#include <libpq-fe.h>
#include "MVC/Tables.h"

class SampleDataStorage
{
public:
	// Data
	bool LoadFiles();
	std::vector<std::string> LoadFile(const std::string& fileName);

	// Tables
	bool CreateTables();
	bool CreateTable(AuxiliaryTable tableName, std::vector<std::string> data);


	void SetConnection(PGconn* connection);
public:
	std::vector<std::string>& GetNames() { return m_Names; }
	std::vector<std::string>& GetSurnames() { return m_Surnames; }
	std::vector<std::string>& GetGenres() { return m_Genres; }
	std::vector<std::string>& GetLabels() { return m_Labels; }
	std::vector<std::string>& GetLocations() { return m_Locations; }
	std::vector<std::string>& GetWords() { return m_Words; }
private:

	std::vector<std::string> m_Names;
	std::vector<std::string> m_Surnames;
	std::vector<std::string> m_Genres;
	std::vector<std::string> m_Labels;
	std::vector<std::string> m_Locations;
	std::vector<std::string> m_Words;	

	PGconn* m_Connection;
};