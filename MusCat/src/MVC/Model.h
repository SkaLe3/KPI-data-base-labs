#pragma once
#include <libpq-fe.h>
#include <memory>
#include <string>
#include "Tables.h"
#include "Model/SampleDataStorage.h"
#include "Model/TestDataGenerator.h"

class Model
{
public:
	~Model();
	void Finish();
	bool Connect(const std::string& username, const std::string& password, std::string& errorMessage);

	bool CreateTables(std::string& errorMessage);

	std::shared_ptr<TableData> FetchTableData(Table table, std::vector<std::string> pkeysTitles, std::string& errorMessage);
	bool CreateRecord(Table table, std::vector<std::string> recordData, std::string& errorMessage);
	bool UpdateRecord(Table table, std::vector<std::string> recordData, std::vector<Column> pkeyColumns, std::vector<std::string> pkeysData, std::string& errorMessage);
	bool DeleteRecord(Table table, std::vector<Column> pkeyColumns, std::vector<std::string> pkeysData, std::string& errorMessage);

	std::vector<std::string> GetRecordIfExists(Table table, std::vector<Column> pkeyColumns, std::vector<std::string> pkeysData, std::string& errorMessage);
	std::vector<std::string> GetPKeyColumnTitles(Table table, std::string& errorMessage);

	bool GenerateData(Table table,int32_t rowCount, std::string& errorMessage);
	bool CheckTableMinRecords(Table table, int32_t count);
public:
	// Test Data
	bool LoadTestDataSamples(std::string& errorMessage);
	bool CreateAuxiliaryTablesForTestData(std::string& errorMessage);

private:
	std::string CreateTableGenre();
	std::string CreateTableLabel();
	std::string CreateTableSong();
	std::string CreateTableAlbum();
	std::string CreateTablePerson();
	std::string CreateTableArtist();
	std::string CreateTableArtist_Person();
	std::string CreateTableArtist_Song();


	std::string CheckCreateResult(PGresult* res, ExecStatusType status, const std::string& text);
private:
	PGconn* m_Connection;

	SampleDataStorage m_SampleDataStorage;
	TestDataGenerator m_Generator;

};

