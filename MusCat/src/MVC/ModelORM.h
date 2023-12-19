#pragma once
#include "ModelBase.h"
#include "orm/tiny/model.hpp"
#include "Models/GenreModel.h"
#include "Models/LabelModel.h"
#include "Models/AlbumModel.h"
#include "Models/SongModel.h"
#include "Models/PersonModel.h"
#include "Models/ArtistModel.h"
#include <unordered_map>

namespace Orm {
class DatabaseManager;
}

class ModelORM final : public ModelBase
{
public:
	ModelORM();
	virtual bool Connect(const std::string& dbname, const std::string& username, const std::string& password, std::string& errorMessage) override;
	virtual std::shared_ptr<TableData> FetchTableData(Table table, std::vector<std::string> pkeysTitles, std::string& errorMessage) override;
	virtual bool CreateRecord(Table table, std::vector<std::string> recordData, std::string& errorMessage) override;
	virtual bool UpdateRecord(Table table, std::vector<std::string> recordData, std::vector<Column> pkeyColumns, std::vector<std::string> pkeysData, std::string& errorMessage) override;
	virtual bool DeleteRecord(Table table, std::vector<Column> pkeyColumns, std::vector<std::string> pkeysData, std::string& errorMessage);

private:
	/* CRUD */
	std::shared_ptr<TableData> FetchGenres(std::string& errorMessage);
	std::shared_ptr<TableData> FetchLabels(std::string& errorMessage);
	std::shared_ptr<TableData> FetchAlbums(std::string& errorMessage);
	std::shared_ptr<TableData> FetchSongs(std::string& errorMessage);
	std::shared_ptr<TableData> FetchPersons(std::string& errorMessage);
	std::shared_ptr<TableData> FetchArtists(std::string& errorMessage);
	std::shared_ptr<TableData> FetchArtistPersons(std::string& errorMessage);
	std::shared_ptr<TableData> FetchArtistSongs(std::string& errorMessage); 

	void CreateGenreRecord(std::vector<std::string> recordData, std::string& errorMessage);
	void CreateLabelRecord(std::vector<std::string> recordData, std::string& errorMessage);
	void CreateAlbumRecord(std::vector<std::string> recordData, std::string& errorMessage);
	void CreateSongRecord(std::vector<std::string> recordData, std::string& errorMessage);
	void CreatePersonRecord(std::vector<std::string> recordData, std::string& errorMessage);
	void CreateArtistRecord(std::vector<std::string> recordData, std::string& errorMessage);
	void CreateArtistPersonRecord(std::vector<std::string> recordData, std::string& errorMessage);
	void CreateArtistSongRecord(std::vector<std::string> recordData, std::string& errorMessage);

	void UpdateGenreRecord(std::vector<std::string> recordData, std::vector<std::string> pkeysData, std::string& errorMessage);
	void UpdateLabelRecord(std::vector<std::string> recordData, std::vector<std::string> pkeysData, std::string& errorMessage);
	void UpdateAlbumRecord(std::vector<std::string> recordData, std::vector<std::string> pkeysData, std::string& errorMessage);
	void UpdateSongRecord(std::vector<std::string> recordData, std::vector<std::string> pkeysData, std::string& errorMessage);
	void UpdatePersonRecord(std::vector<std::string> recordData, std::vector<std::string> pkeysData, std::string& errorMessage);
	void UpdateArtistRecord(std::vector<std::string> recordData, std::vector<std::string> pkeysData, std::string& errorMessage);
	void UpdateArtistPersonRecord(std::vector<std::string> recordData, std::vector<std::string> pkeysData, std::string& errorMessage);
	void UpdateArtistSongRecord(std::vector<std::string> recordData, std::vector<std::string> pkeysData, std::string& errorMessage);

	void DeleteGenreRecord(std::vector<std::string> pkeysData, std::string& errorMessage);
	void DeleteLabelRecord(std::vector<std::string> pkeysData, std::string& errorMessage);
	void DeleteAlbumRecord(std::vector<std::string> pkeysData, std::string& errorMessage);
	void DeleteSongRecord(std::vector<std::string> pkeysData, std::string& errorMessage);
	void DeletePersonRecord(std::vector<std::string> pkeysData, std::string& errorMessage);
	void DeleteArtistRecord(std::vector<std::string> pkeysData, std::string& errorMessage);
	void DeleteArtistPersonRecord(std::vector<std::string> pkeysData, std::string& errorMessage);
	void DeleteArtistSongRecord(std::vector<std::string> pkeysData, std::string& errorMessage);



private:
	template <typename T, typename... SpecificTypes>
	inline static constexpr bool IsModel = (std::is_same<T, SpecificTypes>::value || ...);

	template <typename TModel>
	requires IsModel<TModel, Models::Genre, Models::Label, Models::Album, Models::Song, Models::Person, Models::Artist>
	std::shared_ptr<TableData> CollectionToTableData(Orm::Tiny::Types::ModelsCollection<TModel> collection)
	{
		std::shared_ptr<TableData> data = std::make_shared<TableData>();
		for (TModel& model : collection)
		{
			std::vector<std::string> rowData;
			for (int col = 0, size = model.toVector().size(); col < size; ++col)
			{
				rowData.push_back(model.toVector()[col].value.toString().toStdString());
			}
			data->push_back(rowData);
		}
		return data;
	}
private:

	std::shared_ptr<Orm::DatabaseManager> m_Manager;

	std::unordered_map<Table, std::function<std::shared_ptr<TableData>(std::string&)>> m_FetchDataMap;
	std::unordered_map<Table, std::function<void(std::vector<std::string>, std::string&)>> m_CreateRecordMap;
	std::unordered_map<Table, std::function<void(std::vector<std::string>, std::vector<std::string>, std::string&)>> m_UpdateRecordMap;
	std::unordered_map<Table, std::function<void(std::vector<std::string>, std::string&)>> m_DeleteRecordMap;
};
