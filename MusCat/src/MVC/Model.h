#pragma once
#include <libpq-fe.h>
#include <memory>
#include <string>
#include "Tables.h"
class Model
{
public:
	~Model();
	void Finish();
	bool Connect(const std::string& username, const std::string& password, std::string& errorMessage);
	bool CreateTables(std::string& errorMessage);
	std::shared_ptr<TableData> FetchTableData(Table table, std::string& errorMessage);
	bool AddRecord(Table table, std::vector<std::string> data, std::string& errorMessage);

	bool IsRecordExists(Table table, std::vector<Column> columns, std::vector<std::string> data, std::string& errorMessage);
	std::vector<std::string> GetPKeys(Table table, std::string& errorMessage);

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
};

