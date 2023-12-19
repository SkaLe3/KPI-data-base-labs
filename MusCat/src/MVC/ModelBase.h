#pragma once
#include <libpq-fe.h>
#include <memory>
#include <string>
#include "Tables.h"
#include "Model/SampleDataStorage.h"
#include "Model/TestDataGenerator.h"

class ModelBase
{
public:
	virtual ~ModelBase();
	void Finish();
	virtual bool Connect(const std::string& dbname, const std::string& username, const std::string& password, std::string& errorMessage);

	bool CreateTables(std::string& errorMessage);

	virtual std::shared_ptr<TableData> FetchTableData(Table table, std::vector<std::string> pkeysTitles, std::string& errorMessage);
	virtual bool CreateRecord(Table table, std::vector<std::string> recordData, std::string& errorMessage);
	virtual bool UpdateRecord(Table table, std::vector<std::string> recordData, std::vector<Column> pkeyColumns, std::vector<std::string> pkeysData, std::string& errorMessage);
	virtual bool DeleteRecord(Table table, std::vector<Column> pkeyColumns, std::vector<std::string> pkeysData, std::string& errorMessage);

	std::vector<std::string> GetRecordIfExists(Table table, std::vector<Column> pkeyColumns, std::vector<std::string> pkeysData, std::string& errorMessage);
	std::vector<std::string> GetPKeyColumnTitles(Table table, std::string& errorMessage);

	bool GenerateData(Table table,int32_t rowCount, std::string& errorMessage);
	bool CheckTableMinRecords(Table table, int32_t count);

	// Selections
	std::vector<std::string> GetListOfGenres(std::string& errorMessage);
	std::pair<std::shared_ptr<TableData>, std::vector<std::string>> ExecuteFirstSelection(int32_t age, int32_t songCount,int32_t genreId, std::string genre, std::string& errorMessage);
	std::pair<std::shared_ptr<TableData>, std::vector<std::string>> ExecuteSecondSelection(const std::string& fromDate, const std::string& toDate, std::string& errorMessage);
	std::pair<std::shared_ptr<TableData>, std::vector<std::string>> ExecuteThirdSelection(const std::string& fromDate, const std::string& toDate, std::string& errorMessage);
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

