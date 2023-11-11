#pragma once
#include <vector>
#include <string>
#include <libpq-fe.h>
#include "MVC/Tables.h"

class TestDataGenerator
{
public:
	bool GenerateGenres(std::vector<std::string>& genres, std::string& errorMessage);
	bool GenerateLabels(std::vector<std::string>& labels, std::vector<std::string>& locations, std::string& errorMessage);
	bool GenerateAlbums(int32_t rowCount, std::string& errorMessage);
	bool GenerateSong(int32_t rowCount, std::string& errorMessage);
	bool GeneratePerson(int32_t rowCount, std::string& errorMessage);
	bool GenerateArtist(int32_t rowCount, std::string& errorMessage);
	bool GenerateArtist_Person(int32_t rowCount, std::string& errorMessage);
	bool GenerateArtist_Song(int32_t rowCount, std::string& errorMessage);

public:
	bool DeleteData(Table table, std::string& errorMessage);
	void SetConnection(PGconn* connection);
private:

	PGconn* m_Connection;

};