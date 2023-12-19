#include "ModelORM.h"
#include <orm/db.hpp>
#include <iostream>
#include <VieM/Core/Log.h>
#include <orm/tiny/exceptions/relationmappingnotfounderror.hpp>

ModelORM::ModelORM()
{
	m_FetchDataMap[Table::Genre] = std::bind(&ModelORM::FetchGenres, this, std::placeholders::_1);
	m_FetchDataMap[Table::Label] = std::bind(&ModelORM::FetchLabels, this, std::placeholders::_1);
	m_FetchDataMap[Table::Song] = std::bind(&ModelORM::FetchSongs, this, std::placeholders::_1);
	m_FetchDataMap[Table::Album] = std::bind(&ModelORM::FetchAlbums, this, std::placeholders::_1);
	m_FetchDataMap[Table::Person] = std::bind(&ModelORM::FetchPersons, this, std::placeholders::_1);
	m_FetchDataMap[Table::Artist] = std::bind(&ModelORM::FetchArtists, this, std::placeholders::_1);
	m_FetchDataMap[Table::Artist_Person] = std::bind(&ModelORM::FetchArtistPersons, this, std::placeholders::_1);
	m_FetchDataMap[Table::Artist_Song] = std::bind(&ModelORM::FetchArtistSongs, this, std::placeholders::_1);

	m_CreateRecordMap[Table::Genre] = std::bind(&ModelORM::CreateGenreRecord, this, std::placeholders::_1, std::placeholders::_2);
	m_CreateRecordMap[Table::Label] = std::bind(&ModelORM::CreateLabelRecord, this, std::placeholders::_1, std::placeholders::_2);
	m_CreateRecordMap[Table::Song] = std::bind(&ModelORM::CreateSongRecord, this, std::placeholders::_1, std::placeholders::_2);
	m_CreateRecordMap[Table::Album] = std::bind(&ModelORM::CreateAlbumRecord, this, std::placeholders::_1, std::placeholders::_2);
	m_CreateRecordMap[Table::Person] = std::bind(&ModelORM::CreatePersonRecord, this, std::placeholders::_1, std::placeholders::_2);
	m_CreateRecordMap[Table::Artist] = std::bind(&ModelORM::CreateArtistRecord, this, std::placeholders::_1, std::placeholders::_2);
	m_CreateRecordMap[Table::Artist_Person] = std::bind(&ModelORM::CreateArtistPersonRecord, this, std::placeholders::_1, std::placeholders::_2);
	m_CreateRecordMap[Table::Artist_Song] = std::bind(&ModelORM::CreateArtistSongRecord, this, std::placeholders::_1, std::placeholders::_2);

	m_UpdateRecordMap[Table::Genre] = std::bind(&ModelORM::UpdateGenreRecord, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	m_UpdateRecordMap[Table::Label] = std::bind(&ModelORM::UpdateLabelRecord, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	m_UpdateRecordMap[Table::Song] = std::bind(&ModelORM::UpdateSongRecord, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	m_UpdateRecordMap[Table::Album] = std::bind(&ModelORM::UpdateAlbumRecord, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	m_UpdateRecordMap[Table::Person] = std::bind(&ModelORM::UpdatePersonRecord, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	m_UpdateRecordMap[Table::Artist] = std::bind(&ModelORM::UpdateArtistRecord, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	m_UpdateRecordMap[Table::Artist_Person] = std::bind(&ModelORM::UpdateArtistPersonRecord, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	m_UpdateRecordMap[Table::Artist_Song] = std::bind(&ModelORM::UpdateArtistSongRecord, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	
	m_DeleteRecordMap[Table::Genre] = std::bind(&ModelORM::DeleteGenreRecord, this, std::placeholders::_1, std::placeholders::_2);
	m_DeleteRecordMap[Table::Label] = std::bind(&ModelORM::DeleteLabelRecord, this, std::placeholders::_1, std::placeholders::_2);
	m_DeleteRecordMap[Table::Song] = std::bind(&ModelORM::DeleteSongRecord, this, std::placeholders::_1, std::placeholders::_2);
	m_DeleteRecordMap[Table::Album] = std::bind(&ModelORM::DeleteAlbumRecord, this, std::placeholders::_1, std::placeholders::_2);
	m_DeleteRecordMap[Table::Person] = std::bind(&ModelORM::DeletePersonRecord, this, std::placeholders::_1, std::placeholders::_2);
	m_DeleteRecordMap[Table::Artist] = std::bind(&ModelORM::DeleteArtistRecord, this, std::placeholders::_1, std::placeholders::_2);
	m_DeleteRecordMap[Table::Artist_Person] = std::bind(&ModelORM::DeleteArtistPersonRecord, this, std::placeholders::_1, std::placeholders::_2);
	m_DeleteRecordMap[Table::Artist_Song] = std::bind(&ModelORM::DeleteArtistSongRecord, this, std::placeholders::_1, std::placeholders::_2);

}

bool ModelORM::Connect(const std::string& dbname, const std::string& username, const std::string& password, std::string& errorMessage)
{
	bool success = ModelBase::Connect(dbname, username, password, errorMessage);
	m_Manager = Orm::DB::create({
		{"driver", "QPSQL"},
		{"host", qEnvironmentVariable("DB_HOST", "127.0.0.1")},
		{"port", qEnvironmentVariable("DB_PORT", "5432")},
		{"database", qEnvironmentVariable("DB_DATABASE", dbname.c_str())},
		{"username", qEnvironmentVariable("DB_USERNAME", username.c_str())},
		{"password", qEnvironmentVariable("DB_PASSWORD", password.c_str())},
		});
	return success;
}

std::shared_ptr<TableData> ModelORM::FetchTableData(Table table, std::vector<std::string> pkeysTitles, std::string& errorMessage)
{
	std::shared_ptr<TableData> data;
	try 
	{
		data = m_FetchDataMap[table](errorMessage);
	}
	catch (Orm::Exceptions::QueryError err)
	{
		errorMessage = "Fetching data failed for table " + TableSpecs::GetName(table) + "\n"
			+ err.message().toStdString();
		VM_ERROR(errorMessage);
		return std::make_shared<TableData>();
	}
	return data;
}

bool ModelORM::CreateRecord(Table table, std::vector<std::string> recordData, std::string& errorMessage)
{
	try 
	{
		m_CreateRecordMap[table](recordData, errorMessage);
	}
	catch (Orm::Exceptions::QueryError err)
	{
		errorMessage = "INSERT Query execution failed for table: " + TableSpecs::GetName(table) + "\n" 
			+ err.message().toStdString();
		VM_ERROR(errorMessage);
		return false;
	}
	catch (Orm::Tiny::Exceptions::RelationMappingNotFoundError err)
	{
		errorMessage = "INSERT Query execution failed for table: " + TableSpecs::GetName(table) + "\n"
			+ err.message().toStdString();
		VM_ERROR(errorMessage);
		return false;
	}

	return true;
}

bool ModelORM::UpdateRecord(Table table, std::vector<std::string> recordData, std::vector<Column> pkeyColumns, std::vector<std::string> pkeysData, std::string& errorMessage)
{
	try
	{
		m_UpdateRecordMap[table](recordData, pkeysData, errorMessage);
	}
	catch (Orm::Exceptions::QueryError err)
	{
		errorMessage = "UPDATE Query execution failed for table: " + TableSpecs::GetName(table) + "\n"
			+ err.message().toStdString();
		VM_ERROR(errorMessage);
		return false;
	}
	return true;
}

bool ModelORM::DeleteRecord(Table table, std::vector<Column> pkeyColumns, std::vector<std::string> pkeysData, std::string& errorMessage)
{
	try
	{
		m_DeleteRecordMap[table](pkeysData, errorMessage);
	}
	catch (Orm::Exceptions::QueryError err)
	{
		errorMessage = "DELETE Query execution failed for table: " + TableSpecs::GetName(table) + "\n"
			+ err.message().toStdString();
		VM_ERROR(errorMessage);
		return false;
	}
	return true;
}

std::shared_ptr<TableData> ModelORM::FetchGenres(std::string& errorMessage)
{
	Orm::Tiny::Types::ModelsCollection<Models::Genre> data1 = Models::Genre::all();
	return CollectionToTableData<Models::Genre>(data1);
}

std::shared_ptr<TableData> ModelORM::FetchLabels(std::string& errorMessage)
{
	Orm::Tiny::Types::ModelsCollection<Models::Label> data1 = Models::Label::all();
	return CollectionToTableData(data1);
}

std::shared_ptr<TableData> ModelORM::FetchAlbums(std::string& errorMessage)
{
	Orm::Tiny::Types::ModelsCollection<Models::Album> data1 = Models::Album::all();
	return CollectionToTableData(data1);
}

std::shared_ptr<TableData> ModelORM::FetchSongs(std::string& errorMessage)
{
	Orm::Tiny::Types::ModelsCollection<Models::Song> data1 = Models::Song::all();
	return CollectionToTableData(data1);
}

std::shared_ptr<TableData> ModelORM::FetchPersons(std::string& errorMessage)
{
	Orm::Tiny::Types::ModelsCollection<Models::Person> data1 = Models::Person::all();
	return CollectionToTableData(data1);
}

std::shared_ptr<TableData> ModelORM::FetchArtists(std::string& errorMessage)
{
	Orm::Tiny::Types::ModelsCollection<Models::Artist> data1 = Models::Artist::all();
	return CollectionToTableData(data1);
}

std::shared_ptr<TableData> ModelORM::FetchArtistPersons(std::string& errorMessage)
{
	std::shared_ptr<TableData> data = std::make_shared<TableData>();

	auto artists1 = Models::Artist::with("persons")->get();
	for (auto& artist : artists1)
	{
		auto artist_persons = artist.getRelation<Models::Person>("persons");
		if (!artist_persons.empty())
		{
			for (auto* person : artist_persons)
			{
				std::vector<std::string> rowData;
				auto artist_id = person->getRelation<Orm::Tiny::Relations::Pivot, Orm::One>("pivot")->getAttribute("artist_id");
				auto person_id = person->getRelation<Orm::Tiny::Relations::Pivot, Orm::One>("pivot")->getAttribute("person_id");
				rowData.push_back(artist_id.toString().toStdString());
				rowData.push_back(person_id.toString().toStdString());
				data->push_back(rowData);
			}
		}
	}
	return data;
}

std::shared_ptr<TableData> ModelORM::FetchArtistSongs(std::string& errorMessage)
{
	std::shared_ptr<TableData> data = std::make_shared<TableData>();

	auto artists1 = Models::Artist::with("songs")->get();
	for (auto& artist : artists1)
	{
		auto artist_songs = artist.getRelation<Models::Song>("songs");
		if (!artist_songs.empty())
			for (auto* song : artist_songs)
			{
				std::vector<std::string> rowData;
				auto artist_id = song->getRelation<Orm::Tiny::Relations::Pivot, Orm::One>("pivot")->getAttribute("artist_id");
				auto song_id = song->getRelation<Orm::Tiny::Relations::Pivot, Orm::One>("pivot")->getAttribute("song_id");
				rowData.push_back(artist_id.toString().toStdString());
				rowData.push_back(song_id.toString().toStdString());
				data->push_back(rowData);
			}
	}
	return data;
}

void ModelORM::CreateGenreRecord(std::vector<std::string> recordData, std::string& errorMessage)
{
	Models::Genre genre;
	if (!recordData[1].empty()) genre.setAttribute("name", recordData[1].c_str());
	genre.save();
}

void ModelORM::CreateLabelRecord(std::vector<std::string> recordData, std::string& errorMessage)
{
	Models::Label label;
	if (!recordData[1].empty()) label.setAttribute("name", recordData[1].c_str());
	if (!recordData[2].empty()) label.setAttribute("location", recordData[2].c_str());
	if (!recordData[3].empty()) label.setAttribute("founded_date", recordData[3].c_str());
	label.save();
}

void ModelORM::CreateAlbumRecord(std::vector<std::string> recordData, std::string& errorMessage)
{
	Models::Album album;
	if (!recordData[1].empty()) album.setAttribute("title", recordData[1].c_str());
	album.save();
}

void ModelORM::CreateSongRecord(std::vector<std::string> recordData, std::string& errorMessage)
{
	Models::Song song;
	if (!recordData[1].empty()) song.setAttribute("title", recordData[1].c_str());
	if (!recordData[2].empty()) song.setAttribute("release_date", recordData[2].c_str());
	if (!recordData[3].empty()) song.setAttribute("label_id", recordData[3].c_str());
	if (!recordData[4].empty()) song.setAttribute("genre_id", recordData[4].c_str());
	if (!recordData[5].empty()) song.setAttribute("album_id", recordData[5].c_str());
	song.save();
}

void ModelORM::CreatePersonRecord(std::vector<std::string> recordData, std::string& errorMessage)
{
	Models::Person person;
	if (!recordData[1].empty()) person.setAttribute("name", recordData[1].c_str());
	if (!recordData[2].empty()) person.setAttribute("birth_date", recordData[2].c_str());
	if (!recordData[3].empty()) person.setAttribute("death_date", recordData[3].c_str());
	person.save();
}

void ModelORM::CreateArtistRecord(std::vector<std::string> recordData, std::string& errorMessage)
{
	Models::Artist artist;
	if (!recordData[1].empty()) artist.setAttribute("title", recordData[1].c_str());
	if (!recordData[2].empty()) artist.setAttribute("description", recordData[2].c_str());
	if (!recordData[3].empty()) artist.setAttribute("founded_date", recordData[3].c_str());
	if (!recordData[4].empty()) artist.setAttribute("closed_date", recordData[4].c_str());
	if (!recordData[5].empty()) artist.setAttribute("label_id", recordData[5].c_str());
	artist.save();
}

void ModelORM::CreateArtistPersonRecord(std::vector<std::string> recordData, std::string& errorMessage)
{
	int32_t artist_id = std::stoi(recordData[0]);
	int32_t person_id = std::stoi(recordData[1]);
	auto artist = Models::Artist::find(artist_id);
	artist->persons()->attach(person_id);
}

void ModelORM::CreateArtistSongRecord(std::vector<std::string> recordData, std::string& errorMessage)
{
	int32_t artist_id = std::stoi(recordData[0]);
	int32_t song_id = std::stoi(recordData[1]);
	auto artist = Models::Artist::find(artist_id);
	artist->songs()->attach(song_id);
}

void ModelORM::UpdateGenreRecord(std::vector<std::string> recordData, std::vector<std::string> pkeysData, std::string& errorMessage)
{
	auto genre = Models::Genre::find(std::stoi(pkeysData[0]));
	if (!recordData[1].empty()) genre->setAttribute("name", recordData[1].c_str());
	genre->save();
}

void ModelORM::UpdateLabelRecord(std::vector<std::string> recordData, std::vector<std::string> pkeysData, std::string& errorMessage)
{
	auto label = Models::Label::find(std::stoi(pkeysData[0]));
	if (!recordData[1].empty()) label->setAttribute("name", recordData[1].c_str());
	if (!recordData[2].empty()) label->setAttribute("location", recordData[2].c_str());
	if (!recordData[3].empty()) label->setAttribute("founded_date", recordData[3].c_str());
	label->save();
}

void ModelORM::UpdateAlbumRecord(std::vector<std::string> recordData, std::vector<std::string> pkeysData, std::string& errorMessage)
{
	auto album = Models::Album::find(std::stoi(pkeysData[0]));
	if (!recordData[1].empty()) album->setAttribute("title", recordData[1].c_str());
	album->save();
}

void ModelORM::UpdateSongRecord(std::vector<std::string> recordData, std::vector<std::string> pkeysData, std::string& errorMessage)
{
	auto song = Models::Song::find(std::stoi(pkeysData[0]));
	if (!recordData[1].empty()) song->setAttribute("title", recordData[1].c_str());
	if (!recordData[2].empty()) song->setAttribute("release_date", recordData[2].c_str());
	if (!recordData[3].empty()) song->setAttribute("label_id", recordData[3].c_str());
	if (!recordData[4].empty()) song->setAttribute("genre_id", recordData[4].c_str());
	if (!recordData[5].empty()) song->setAttribute("album_id", recordData[5].c_str());
	song->save();
}

void ModelORM::UpdatePersonRecord(std::vector<std::string> recordData, std::vector<std::string> pkeysData, std::string& errorMessage)
{
	auto person = Models::Person::find(std::stoi(pkeysData[0]));
	if (!recordData[1].empty()) person->setAttribute("name", recordData[1].c_str());
	if (!recordData[2].empty()) person->setAttribute("birth_date", recordData[2].c_str());
	if (!recordData[3].empty()) person->setAttribute("death_date", recordData[3].c_str());
	person->save();
}

void ModelORM::UpdateArtistRecord(std::vector<std::string> recordData, std::vector<std::string> pkeysData, std::string& errorMessage)
{
	auto artist = Models::Artist::find(std::stoi(pkeysData[0]));
	if (!recordData[1].empty()) artist->setAttribute("title", recordData[1].c_str());
	if (!recordData[2].empty()) artist->setAttribute("description", recordData[2].c_str());
	if (!recordData[3].empty()) artist->setAttribute("founded_date", recordData[3].c_str());
	if (!recordData[4].empty()) artist->setAttribute("closed_date", recordData[4].c_str());
	if (!recordData[5].empty()) artist->setAttribute("label_id", recordData[5].c_str());
	artist->save();
}

void ModelORM::UpdateArtistPersonRecord(std::vector<std::string> recordData, std::vector<std::string> pkeysData, std::string& errorMessage)
{
	DeleteArtistPersonRecord(pkeysData, errorMessage);
	CreateArtistPersonRecord(recordData, errorMessage);
}

void ModelORM::UpdateArtistSongRecord(std::vector<std::string> recordData, std::vector<std::string> pkeysData, std::string& errorMessage)
{
	DeleteArtistSongRecord(pkeysData, errorMessage);
	CreateArtistSongRecord(recordData, errorMessage);
}

void ModelORM::DeleteGenreRecord(std::vector<std::string> pkeysData, std::string& errorMessage)
{
	Models::Genre::destroy(std::stoi(pkeysData[0]));
}

void ModelORM::DeleteLabelRecord(std::vector<std::string> pkeysData, std::string& errorMessage)
{
	Models::Label::destroy(std::stoi(pkeysData[0]));
}

void ModelORM::DeleteAlbumRecord(std::vector<std::string> pkeysData, std::string& errorMessage)
{
	Models::Album::destroy(std::stoi(pkeysData[0]));
}

void ModelORM::DeleteSongRecord(std::vector<std::string> pkeysData, std::string& errorMessage)
{
	Models::Song::destroy(std::stoi(pkeysData[0]));
}

void ModelORM::DeletePersonRecord(std::vector<std::string> pkeysData, std::string& errorMessage)
{
	Models::Person::destroy(std::stoi(pkeysData[0]));
}

void ModelORM::DeleteArtistRecord(std::vector<std::string> pkeysData, std::string& errorMessage)
{
	Models::Artist::destroy(std::stoi(pkeysData[0]));
}

void ModelORM::DeleteArtistPersonRecord(std::vector<std::string> pkeysData, std::string& errorMessage)
{
	int32_t artist_id = std::stoi(pkeysData[0]);
	int32_t person_id = std::stoi(pkeysData[1]);
	auto artist = Models::Artist::find(artist_id);
	artist->songs()->detach(person_id, false);
}

void ModelORM::DeleteArtistSongRecord(std::vector<std::string> pkeysData, std::string& errorMessage)
{
	int32_t artist_id = std::stoi(pkeysData[0]);
	int32_t song_id = std::stoi(pkeysData[1]);
	auto artist = Models::Artist::find(artist_id);
	artist->songs()->detach(song_id, false);
}

