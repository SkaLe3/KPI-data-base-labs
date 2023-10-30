#pragma once
#include <libpq-fe.h>
#include <string>
class Model
{
public:
	~Model();
	void Finish();
	bool Connect(const std::string& username, const std::string& password, std::string& errorMessage);
	bool CreateTables(std::string& errorMessage);
private:
	std::string CreateTableGenre();
	std::string CreateTableLabel();
	std::string CreateTableSong();
	std::string CreateTableAlbum();
	std::string CreateTablePerson();
	std::string CreateTableArtist();
	std::string CreateTableArtist_Person();
	std::string CreateTableArtist_Song();


	std::string CheckResult(PGresult* res, ExecStatusType status, const std::string& text);
private:
	PGconn* m_Connection;
};

